import string
import numpy
from sklearn import tree


samples = []

single_value = "0-7-1|1-19-0|0-7-0|0-0-0|0-11-0|0-0-0|0-2-0|0-0-0|0-2-0|0-0-0|0-4-1|0-1-0|1-10-1|1-3-0"

def state_str_to_array(state) : 
    
    a_list = state.split("|")
    map_object = map(str, a_list)
    list_of_strs = list(map_object)
    
    value = []
    for element in list_of_strs :
        list_b = element.split("-")
        map_b = map(int, list_b)
        value.extend(list(map_b))
    return value


with open("wins.txt") as file :
    wins = file.read().splitlines()

with open("defeats.txt") as file : 
    defeats = file.read().splitlines()
with open("slight_disadvantages.txt") as file : 
    slight_disadvantages = file.read().splitlines()
with open("slight_advantages.txt") as file : 
    slight_advantages = file.read().splitlines()
with open("large_disadvantages.txt") as file : 
    large_disadvantages = file.readlines()
with open("large_advantages.txt") as file : 
    large_advantages = file.read().splitlines()

samples.extend(wins)
samples.extend(defeats)
samples.extend(slight_disadvantages)
samples.extend(slight_advantages)
samples.extend(large_disadvantages)
samples.extend(large_advantages)
state_labels = [0 for i in range (len(wins))]
state_labels.extend([1 for i in range (len(defeats))])
state_labels.extend([2 for i in range (len(slight_disadvantages))])
state_labels.extend([3 for i in range (len(slight_advantages))])
state_labels.extend([4 for i in range (len(large_disadvantages))])
state_labels.extend([5 for i in range (len(large_advantages))])

string_labels = ["win", "defeat", "slight disadvantage", "slight advantage", "large disadvantage", "large advantage"]

print(len(samples))
for index, line in enumerate(samples) : 
    a_list = line.split("|")
    map_object = map(str, a_list)
    list_of_integers = list(map_object)
    
    value = []
    for element in list_of_integers :
        list_b = element.split("-")
        map_b = map(int, list_b)
        value.extend(list(map_b))
    samples[index] = value

print(len(samples), len(state_labels), len(defeats)+ len(wins) + len(large_advantages) +
len(large_disadvantages) + len(slight_advantages) + len(slight_disadvantages))
clf = tree.DecisionTreeClassifier()
clf = clf.fit(samples, state_labels)

test_array = [state_str_to_array(single_value)]
print(test_array)
print(clf.predict(test_array))