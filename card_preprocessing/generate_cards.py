import os
import sys
import json

PATH = 'cards_included.json'
print(f'Parsing cards from: {PATH}')
with open(PATH, 'r') as f: cards = json.load(f)
print('Cards parsed into \'cards\'.')

def generate_add_tags(tags):
    out = ""
    for tag in tags:
        out = out + f"\n    AddTag( Tag::{tag.upper()} );"
    return out + "\n"

def generate_event_or_automated_card(card):
    snake_case_name = card["name"].lower().replace("-", "_").replace(" ", "_")
    pascal_case_name = card["name"].replace("-", " ").title().replace(" ", "")
    upper_case_name = snake_case_name.upper()

    type = card["type"].lower()
    
    tags = card["tags"] if "tags" in card else []
    is_building = "building" in tags
    is_space = "space" in tags

    add_tags = "" if len(tags) == 0 else generate_add_tags(tags)

    requirements = []

    loses = []

    benefits = []

    actions = []

    if "requirements" in card:
        reqs = card["requirements"]
        
        if "min_temperature" in reqs: requirements.append(f"_model.Temperature() >= {reqs["min_temperature"]}")
        if "max_temperature" in reqs: requirements.append(f"_model.Temperature() <= {reqs["max_temperature"]}")
        if "min_oceans" in reqs: requirements.append(f"_model.OceanCount() >= {reqs["min_oceans"]}")
        if "max_oceans" in reqs: requirements.append(f"_model.OceanCount() <= {reqs["max_oceans"]}")
        if "min_oxygen" in reqs: requirements.append(f"_model.Oxygen() >= {reqs["min_oxygen"]}")
        if "max_oxygen" in reqs: requirements.append(f"_model.Oxygen() <= {reqs["max_oxygen"]}")

        if "tags" in reqs:
            for tag, count in reqs["tags"].items():
                requirements.append(f"_owner->GetTagCount( Tag::{tag.upper()} ) >= {count}")
    
    if "draw_cards" in card:
        for _ in range(card["draw_cards"]):
            benefits.append("\n    _owner->DrawCard();")
    
    if "raise_tr" in card:
        benefits.append(f"\n    _owner->RaiseTR( {card["raise_tr"]} );")

    vps = card["gain_vps"] if "gain_vps" in card else 0

    if "raise_temp" in card:
        for _ in range(card["raise_temp"]):
            benefits.append("\n    _owner->RaiseTemperature();")

    if "place_ocean" in card:
        for _ in range(card["place_ocean"]):
            actions.append("\n    _owner->PlaceOcean();")

    if "raise_oxygen" in card:
        for _ in range(card["raise_oxygen"]):
            benefits.append("\n    _owner->RaiseOxygen();")
    
    if "place_greenery" in card:
        for _ in range(card["place_greenery"]):
            requirements.append("_model.IsTilePlaceable()")
            actions.append("\n    _owner->PlaceGreenery();")

    if "place_city" in card:
        for _ in range(card["place_city"]):
            requirements.append("_model.IsCityPlaceable()")
            actions.append("\n    _owner->PlaceCity();")
    
    if "gain_resource" in card:
        for resource, count in card["gain_resource"].items():
            benefits.append(f"\n    _owner->GainResource( Resource::{resource.upper()}, {count} );")
    
    if "lose_resource" in card:
        for resource, count in card["lose_resource"].items():
            requirements.append(f"_owner->GetResource( Resource::{resource.upper()} ) >= {count}")
            loses.append(f"\n    _owner->LoseResource( Resource::{resource.upper()}, {count} );")
    
    if "destroy_resource" in card:
        for resource, count in card["destroy_resource"].items():
            benefits.append(f"\n    _owner->DestroyResource( Resource::{resource.upper()}, {count} );")
    
    if "gain_resource_production" in card:
        for resource, count in card["gain_resource_production"].items():
            benefits.append(f"\n    _owner->GainResourceProduction( Resource::{resource.upper()}, {count} );")
    
    if "lose_resource_production" in card:
        for resource, count in card["lose_resource_production"].items():
            requirements.append(f"_owner->GetResourceProduction( Resource::{resource.upper()} ) >= {count}")
            loses.append(f"\n    _owner->LoseResourceProduction( Resource::{resource.upper()}, {count} );")
    
    if "destroy_resource_production" in card:
        for resource, count in card["destroy_resource_production"].items():
            benefits.append(f"\n    _owner->DestroyResourceProduction( Resource::{resource.upper()}, {count} );")
    
    if "add_resource" in card:
        for resource, count in card["add_resource"].items():
            benefits.append(f"\n    // _owner->AddResource( Resource::{resource.upper()}, {count} );")


    if len(requirements) == 0:
        satisfies_requirements_declaration = ""
        satisfies_requirements = ""
    else:
        satisfies_requirements_declaration = "\n    bool SatisfiesRequirements() const override;"
        satisfies_requirements = f"""

bool {pascal_case_name}::SatisfiesRequirements() const {{
    return {" && ".join(requirements)};
}}"""

    apply_effects = "\n".join(filter(lambda s: len(s) > 0, ["".join(loses), "".join(benefits), "".join(actions)]))

    if vps == 0:
        count_vps_declaration = ""
        count_vps = ""
    else:
        count_vps_declaration = "\n    int DoCountVPs() const override;"
        count_vps = f"""

int {pascal_case_name}::DoCountVPs() const {{
    return {vps};
}}"""


    header = f'''#pragma once

#include "../{type}_card.h"
#include "../game_model.h"

namespace model::decks::cards
{{
class {pascal_case_name} : public {type.title()}Card
{{
public:
    {pascal_case_name}( const GameModel& model ) noexcept;
    ~{pascal_case_name}() noexcept;

protected:{satisfies_requirements_declaration}
    void ApplyImmediateEffects() override;{count_vps_declaration}
}};
}}
'''

    code = f'''#include "{snake_case_name}.h"

#include "../{type}_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{{
{pascal_case_name}::{pascal_case_name}( const GameModel& model ) noexcept :
    {type.title()}Card( model, CardID::{upper_case_name}, {card["cost"]}, {f"{is_building}".lower()}, {f"{is_space}".lower()} ) {{{add_tags}}}

{pascal_case_name}::~{pascal_case_name}() noexcept {{}}{satisfies_requirements}

void {pascal_case_name}::ApplyImmediateEffects() {{{apply_effects}
}}{count_vps}
}}
'''

    with open(f"cards/{snake_case_name}.h", "w") as f: f.write(header)
    with open(f"cards/{snake_case_name}.cpp", "w") as f: f.write(code)
    print(f"{card["name"]} generated successfully")


# cards = [
#     {
#         'id': 100002,
#         'name': 'Sample Card II',
#         'type': 'event',
#         'cost': 5,
#         'gain_resource': {'credit': 5}
#     },
#     {
#         'id': 100001,
#         'name': 'Sample Card I',
#         'type': 'automated',
#         'cost': 15,
#         'tags': ['building', 'power', 'earth'],
#         'requirements': {
#             'min_oxygen': 7,
#             'max_oceans': 5,
#             'tags': {'science': 2, 'building': 1},
#         },
#         'draw_cards': 2,
#         'raise_tr': 1,
#         'gain_vps': -2,
#         'raise_temp': 2,
#         'place_ocean': 1,
#         'raise_oxygen': 3,
#         'place_greenery': 1,
#         'place_city': 1,
#         'gain_resource': {'steel': 2, 'titanium': 1},
#         'lose_resource': {'plants': 1},
#         'desroy_resource': {'plants': 1},
#         'gain_resource_production': {'energy': 2, 'plants': 1},
#         'lose_resource_production': {'plants': 1},
#         'desroy_resource_production': {'plants': 1, 'heat': 2},
#         'add_resource': {'animals': 2}
#     }
# ]

cards = filter(lambda c: c["id"] in [16, 29, 32, 108], cards)

os.makedirs("cards", exist_ok=True)
errors = 0

for card in cards:
    if card["type"] == "active":
        print(f"{card["name"]} with ID: {id} is an active card!", file=sys.stderr)
        continue

    try:
        generate_event_or_automated_card(card)
    except Exception as e:
        print(f"ERROR: Failed at {card["name"]}: {e} ")
        errors = errors + 1

print(f"Error count: {errors}")
