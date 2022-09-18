from grid_map import GridMap, GridMapIterator
import numpy as np

def gridMapIterator():
    map = GridMap()
    map.setGeometry(length=np.array([8.1, 5.1]), resolution=1.0, position=np.array([0.0, 0.0])) # buffer size is [8, 5]
    map.add("layer", 0.0)
    iterator = GridMapIterator(map)

    i = 0
    while not iterator.isPastEnd():
        map.setValueAt(layer="layer", index=iterator.getIndex(), value=1.0) 
        iterator.increment()
        i += 1

    print('\n=============== Test GridMapIterator ===============')
    print('i: ', 40)
    print('iterator.isPastEnd(): ', iterator.isPastEnd())
    print('map["layer"]: \n', map["layer"])


gridMapIterator()