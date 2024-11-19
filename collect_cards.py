import json
import traceback
import sys

CARD_FILE = 'cards.json'
READ_CARDS = True
READ_CARDS_FILE = 'cards.json'
ERROR_LOG_FILE = 'cards_error.json'

if len(sys.argv) > 1:
    READ_CARDS_FILE = sys.argv[1]

MAIN_MENU = '''
n   - new card
p   - print all cards
pl  - print last card
pid - print card by id
e   - edit card
d   - delete last card
s   - save
    - quit and save
qns - quit no save
>>> '''
GET_CARD_NUM = 'Enter card id: '
GET_CARD_NAME = 'Enter card name: '
GET_CARD_TYPE = 'Enter card type (automated, active, event): '
GET_CARD_COST = 'Enter card cost: '
CARD_MENU = '''
h - print help
s - show card
  - save card
q - discard card
>>> '''
GET_NEXT_ADD = '''
at     - add tag
ar     - add requirement
adc    - add draw card
atr    - add terraform rating increase
avp    - add VPs increase or decrease
avps   - add VPs increase with special conditions
art    - add raise temperature
apoc   - add place ocean
acfoc  - add condition for ocean placement
arox   - add raise oxygen
apg    - add place greenery
acfg   - add condition for greenery placement
apc    - add place city
acfc   - add condition for city placement
agr    - add gain resource
agrs   - add gain resource with special conditions
alr    - add lose resource
adr    - add destroy resource
agrp   - add gain resource production
agrps  - add gain resource production with special conditions
alrp   - add lose resource production
adrp   - add destroy resource production
agroac - add gain resource on another card
amc    - add multiple choice
aa     - add action
ae     - add effect
as     - add special
asct   - add special card type
>>> '''
GET_NEXT_REQ = '''
mint  - add minimum temperature
maxt  - add maximum temperature
minoc - add minimum oceans
maxoc - add maximum oceans
minox - add minimum oxygen
maxox - add maximum oxygen
tag   - add tag requirements
>>> '''

def is_int(s):
    try: 
        int(s)
    except ValueError:
        return False
    else:
        return True

def get_maybe_int(s):
    try: 
        return int(s)
    except ValueError:
        return s

def get_int(msg):
    instr = input(msg)
    while not is_int(instr):
        instr = input(msg)
    return int(instr)

def get_list(msg):
    print(msg)
    l = []
    instr = input('Enter next item: ')
    while instr != '':
        l.append(get_maybe_int(instr))
        instr = input('Enter next item: ')
    return l

def get_dict(msg):
    print(msg)
    d = {}
    instr = input('Enter next key: ')
    while instr != '':
        key = input('Enter value: ')
        d[get_maybe_int(instr)] = get_maybe_int(key)
        instr = input('Enter next key: ')
    return d

def get_dict_with_keys(msg, *args):
    print(f'{msg} ({", ".join(args)})')
    d = {}
    for arg in args:
        instr = input(f'Enter value for {arg}: ')
        d[arg] = get_maybe_int(instr)
    return d


sample_card = {
    'id': 99999,
    'name': 'Sample Card',
    'type': 'automated', # automated, active, event
    'cost': 15,
    'tags': ['building', 'power', 'earth'],
    'requirements': {
        'min_oxygen': 7,
        'max_oceans': 5,
        'tags': {'science': 2, 'building': 1},
    },
    'draw_cards': 2,
    'raise_tr': 1,
    'gain_vps': -2,
    'gain_vps_special': {'amount': 1, 'condition': 'animals on card'},
    'raise_temp': 2, # steps
    'place_ocean': 1,
    'place_ocean_special': 'on non-ocean tile',
    'raise_oxygen': 3,
    'place_greenery': 1,
    'place_greenery_special': 'on ocean tile',
    'place_city': 1,
    'place_city_special': 'on reserved area',
    'gain_resource': {'steel': 2, 'titanium': 1},
    'gain_resource_special': {'resource': 'heat', 'amount': 1, 'condition': 'city'},
    'lose_resource': {'plants': 1},
    'desroy_resource': {'plants': 1},
    'gain_resource_production': {'energy': 2, 'plants': 1},
    'gain_resource_production_special': {'resource': 'energy', 'amount': 1, 'condition': 'power tag'},
    'lose_resource_production': {'plants': 1},
    'desroy_resource_production': {'plants': 1, 'heat': 2},
    'add_resource': {'animals': 2},
    'gain_multiple_choice': {
        'plants': 3,
        'microbe': 3,
        'animal': 2,
    },
    'actions': [
        {'cost': {}, 'then': {'microbe': 1}},
        {'cost': {'microbe': 3}, 'then': {'oxygen': 1}},
        {'cost': {'credit': 7, 'power': 4}, 'then': {'energy production': 1}},
    ],
    'effect': {'when': ['you', 'place', 'greenery'], 'then': {'animal': 1}},
            # {'when': ['anyone', 'place', 'city'], 'then': {'credit production': 1}},
            # {'when': ['you', 'play', 'space', 'event'], 'then': {'heat': 3, 'credit': 3}},
    'special': [
        '',
        '',
    ],
    'special_card_types': [
        'animal',
        'microbe',
        'aura',
        'puts resources on other cards when played',
        'add/steel resources from other cards',
        'may not be included',
    ],
}

instr = '.'
if READ_CARDS:
    print(f'reading cards from: {READ_CARDS_FILE}')
    with open(READ_CARDS_FILE, 'r') as file: cards = json.load(file)
else:
    cards = []


try:
    while instr != 'qns':
        instr = input(MAIN_MENU)
        if instr == 'p': print(json.dumps(cards, indent = 4))
        if instr == 'pl': print(json.dumps(cards[-1], indent = 4))
        if instr == 'pid':
            id = get_int('Enter card id: ')
            for c in cards:
                if c['id'] == id:
                    print(json.dumps(c, indent = 4))
                    break
        if instr == 'e':
            if len(cards) > 0:
                id = input('Enter card id or press enter for last card: ')
                if is_int(id):
                    id = int(id)
                    found = False
                    for c in cards:
                        if c['id'] == id:
                            card = c
                            found = True
                            break
                    if not found:
                        print('Card not found')
                        continue
                    cards.remove(card)
                    print('Editing card with id', id)
                else:
                    card = cards.pop()
                    print('Editing last card')
            else:
                print('No card to edit')
        elif instr == 'd':
            if len(cards) > 0:
                cards.pop()
                print('Deleted last card')
            else: print('No card to delete')
        elif instr == 's' or instr == '':
            cards.sort(key=lambda x: x['id'])
            with open(CARD_FILE, 'w') as file: json.dump(cards, file, indent = 4)
            print('Saved')
            if instr == '': break

        if instr == 'n' or instr == 'e':
            if instr != 'e':
                card = {}
                card['id'] = get_int(GET_CARD_NUM)
                card['name'] = input(GET_CARD_NAME)
                card['type'] = input(GET_CARD_TYPE)
                card['cost'] = get_int(GET_CARD_COST)

            instr = input(GET_NEXT_ADD)
            while instr != '' and instr != 'q':
                try:
                    if instr == 'h': print(GET_NEXT_ADD)
                    elif instr == 's': print(json.dumps(card, indent = 4))

                    elif instr == 'at': card['tags'] = get_list('Enter tags')
                    elif instr == 'ar':
                        if 'requirements' not in card: card['requirements'] = {}
                        instr = input(GET_NEXT_REQ)
                        if   instr == 'mint':  card['requirements']['min_temperature'] = get_int('Add min temperature: ')
                        elif instr == 'maxt':  card['requirements']['max_temperature'] = get_int('Add max temperature: ')
                        elif instr == 'minoc': card['requirements']['min_oceans'] = get_int('Add min oceans: ')
                        elif instr == 'maxoc': card['requirements']['max_oceans'] = get_int('Add max oceans: ')
                        elif instr == 'minox': card['requirements']['min_oxygen'] = get_int('Add min oxygen: ')
                        elif instr == 'maxox': card['requirements']['max_oxygen'] = get_int('Add max oxygen: ')
                        elif instr == 'tag': card['requirements']['tags'] = get_dict('Add tag requirements {tag: amount}')
                    elif instr == 'adc': card['draw_cards'] = get_int('Enter draw card amount: ')
                    elif instr == 'atr': card['raise_tr'] = get_int('Enter tr increase amount: ')
                    elif instr == 'avp': card['gain_vps'] = get_int('Enter VPs increase amount: ')
                    elif instr == 'avps': card['gain_vps_special'] = get_dict_with_keys('Enter VPs increase with special conditions', 'amount', 'condition')
                    elif instr == 'art': card['raise_temp'] = get_int('Enter temperature increase amount: ')
                    elif instr == 'apoc': card['place_ocean'] = get_int('Enter ocean placement amount: ')
                    elif instr == 'acfoc':
                        card['place_ocean'] = get_int('Enter ocean placement amount: ')
                        card['place_ocean_special'] = input('Enter special condition for ocean placement: ')
                    elif instr == 'arox': card['raise_oxygen'] = get_int('Enter oxygen increase amount: ')
                    elif instr == 'apg': card['place_greenery'] = get_int('Enter greenery placement amount: ')
                    elif instr == 'acfg':
                        card['place_greenery'] = get_int('Enter greenery placement amount: ')
                        card['place_greenery_special'] = input('Enter special condition for greenery placement: ')
                    elif instr == 'apc': card['place_city'] = get_int('Enter city placement amount: ')
                    elif instr == 'acfc':
                        card['place_city'] = get_int('Enter city placement amount: ')
                        card['place_city_special'] = input('Enter special condition for city placement: ')
                    elif instr == 'agr': card['gain_resource'] = get_dict('Enter gain resource {resource: amount}')
                    elif instr == 'agrs': card['gain_resource_special'] = get_dict_with_keys('Enter gain resource with special conditions', 'resource', 'amount', 'condition')
                    elif instr == 'alr': card['lose_resource'] = get_dict('Enter lose resource {resource: amount}')
                    elif instr == 'adr': card['destroy_resource'] = get_dict('Enter destroy resource {resource: amount}')
                    elif instr == 'agrp': card['gain_resource_production'] = get_dict('Enter gain resource production {resource: amount}')
                    elif instr == 'agrps': card['gain_resource_production_special'] = get_dict_with_keys('Enter gain resource production with special conditions', 'resource', 'amount', 'condition')
                    elif instr == 'alrp': card['lose_resource_production'] = get_dict('Enter lose resource production {resource: amount}')
                    elif instr == 'adrp': card['destroy_resource_production'] = get_dict('Enter destroy resource production {resource: amount}')
                    elif instr == 'agroac': card['add_resource'] = get_dict('Enter add resource {resource: amount}')
                    elif instr == 'amc': card['gain_multiple_choice'] = get_dict('Enter gain multiple choice {resource: amount}')
                    elif instr == 'aa':
                        l = []
                        for i in range(get_int('Enter number of actions: ')):
                            d = {}
                            for key in ['cost', 'then']:
                                d[key] = get_dict(f'{i + 1}. action: Enter {key} params {{resource: amount}}')
                            l.append(d)
                        card['actions'] = l
                    elif instr == 'ae':
                        d = {}
                        d['when'] = get_list('Enter when conditions')
                        d['then'] = get_dict('Enter then conditions {resource: amount}')
                        card['effect'] = d
                    elif instr == 'as':
                        if 'special' not in card: card['special'] = []
                        card['special'].append(input('Enter special condition: '))
                    elif instr == 'asct':
                        if 'special_card_types' not in card: card['special_card_types'] = []
                        card['special_card_types'].append(input('Enter special card type (animal, microbe, aura, puts resources on other cards when played, add/steel resources from other cards, may not be included): '))
                except KeyboardInterrupt:
                    pass

                instr = input(CARD_MENU)
            if instr == '': cards.append(card)
except KeyboardInterrupt:
    print()
    print(cards)
    print(card)
except Exception as e:
    print()
    print(cards)
    print(card)
    print()
    traceback.print_exc()
    with open(ERROR_LOG_FILE, 'w') as file: json.dump(cards, file)
