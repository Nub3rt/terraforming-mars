import json

# run 'from cardsstat_helper import *' to use this file

PATH = 'cards_included.json'
print(f'Parsing cards from: {PATH}')
with open(PATH, 'r') as f: cards = json.load(f)
print('Cards parsed into \'cards\'.')

def is_of_type(type):
    def iot(card): return card["type"] == type
    return iot

def has_attribute(attribute):
    def ha(card): return attribute in card
    return ha

def has_tags(*tags):
    def ht(card):
        if 'tag' not in card: return False
        for tag in tags:
            if tag not in card['tags']: return False
        return True
    return ht

def negate(f):
    def n(card): return not f(card)
    return n

def microbe(): return lambda card: 'special_card_types' in card and 'microbe' in card['special_card_types']
def animal(): return lambda card: 'special_card_types' in card and 'animal' in card['special_card_types']
def microbes_and_animals(): return lambda card: microbe()(card) or animal()(card)
def maybe_not_included(): return lambda card: 'special_card_types' in card and 'may not be included' in card['special_card_types']


def print_id(id): print(json.dumps(list(filter(lambda c: c['id'] == id,cards))))

def print_cards(cards): print(json.dumps(cards, indent = 4))

def evaluate(f, cards):
    filtered = list(filter(f, cards))
    return filtered

def evaluates(f): return evaluate(f, cards)

def evaluate_str(source, cards):
    def f(card):
        return eval(source)
    return evaluate(f, cards)

def evaluates_str(source): return evaluate_str(source, cards)

def stat_helper(f): return len(list(filter(f, cards)))

def print_stats(): print(f'''
Number of cards: {len(cards)}
Number of automated cards: {stat_helper(lambda c: c['type'] == 'automated')}
Number of active cards: {stat_helper(lambda c: c['type'] == 'active')}
Number of events: {stat_helper(lambda c: c['type'] == 'event')}

Number of 'maybe not included' cards: {stat_helper(lambda c: 'special_card_types' in c and 'may not be included' in c['special_card_types'])}
Number of 'microbe' cards: {stat_helper(lambda c: 'special_card_types' in c and 'microbe' in c['special_card_types'])}
Number of 'maybe not included' AND 'microbe' cards: {stat_helper(lambda c: 'special_card_types' in c and 'may not be included' in c['special_card_types'] and 'microbe' in c['special_card_types'])}
Number of 'animal' cards: {stat_helper(lambda c: 'special_card_types' in c and 'animal' in c['special_card_types'])}
Number of 'maybe not included' AND 'animal' cards: {stat_helper(lambda c: 'special_card_types' in c and 'may not be included' in c['special_card_types'] and 'animal' in c['special_card_types'])}
Number of 'aura' cards: {stat_helper(lambda c: 'special_card_types' in c and 'aura' in c['special_card_types'])}
Number of cards that put resources on other cards when played: {stat_helper(lambda c: 'special_card_types' in c and 'puts resources on other cards when played' in c['special_card_types'])}
Number of cards that add/steel resources from other cards: {stat_helper(lambda c: 'special_card_types' in c and 'add/steel resources from other cards' in c['special_card_types'])}
''')

def print_help(): print('''
Helper functions:
    has_attribute(attribute) - returns a function that returns True when the card has the given attribute
    has_tag(*tags) - returns a function that returns True when the card has all of the given tags
    is_of_type(type) - returns a function that returns True when the card has the given type
    negate(f) - returns a function that negates the return value of f
    microbe() - returns a function that returns True if the card is a microbe
    animal() - returns a function that returns True if the card is an animal
    microbes_and_animals() - returns a function that returns True if the card is a microbe or an animal
    maybe_not_included() - returns a function that returns True if the card is marked as may not be included

    print_id(id) - print card with id

    print_cards(cards) - pretty print the cards

    evaluate(f, cards) - filters cards with the f function, returns them in a list and also prints them prettily
    evaluate_str(source, cards) - same as evaluate, but uses eval(source). use 'card' as the variable for the analyzed card
    evaluates(f), evaluates_str(source) - same as above, uses 'cards' variable as default

    print_help() - prints this text
    print_stats() - prints statistics
''')
print_help()
