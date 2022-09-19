from grid_map import GridMap, GridMapIterator, SlidingWindowIterator
import numpy as np

def testGridMapIterator():
    map = GridMap()
    map.setGeometry(length=np.array([8.1, 5.1]), resolution=1.0, position=np.array([0.0, 0.0])) # buffer size is [8, 5]
    map.add("layer", 0.0)
    iterator = GridMapIterator(gridMap=map)

    i = 0
    while not iterator.isPastEnd():
        map.setValueAt(layer="layer", index=iterator.getIndex(), value=1.0) 
        iterator.increment()
        i += 1

    print('\n=============== Test GridMapIterator ===============')
    print('i: ', i)
    print('iterator.isPastEnd(): ', iterator.isPastEnd())
    print('map["layer"]: \n', map["layer"])

def testSlidingWindowIterator():
    map = GridMap()
    map.setGeometry(length=np.array([8.1, 5.1]), resolution=1.0, position=np.array([0.0, 0.0])) # buffer size is [8, 5]
    map.add("layer")
    map["layer"] = np.random.randn(*map["layer"].shape)

    print('\n=============== Test SlidingWindowIterator ===============')
    iterator = SlidingWindowIterator(gridMap=map, layer="layer", edgeHandling=SlidingWindowIterator.EdgeHandling.CROP, windowSize=3)
    print('iterator.getData().shape: ', iterator.getData().shape)
    print('iterator.getData(): \n', iterator.getData())
    print('map["layer"][0:2, 0:2]: \n', map["layer"][0:2, 0:2])

    iterator.increment()
    print('\niterator.getData().shape: ', iterator.getData().shape)
    print('iterator.getData(): \n', iterator.getData())
    print('map["layer"][0:3, 0:2]: \n', map["layer"][0:3, 0:2])

    iterator.increment()
    print('\niterator.getData().shape: ', iterator.getData().shape)
    print('iterator.getData(): \n', iterator.getData())
    print('map["layer"][1:4, 0:2]: \n', map["layer"][1:4, 0:2])

    while not iterator.isPastEnd():
        iterator.increment()

    print('\niterator.getData().shape: ', iterator.getData().shape)
    print('iterator.getData(): \n', iterator.getData())
    print('map["layer"][6:8, 3:5]: \n', map["layer"][6:8, 3:5])


testGridMapIterator()
testSlidingWindowIterator()