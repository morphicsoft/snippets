class Node: pass

def kdtree(point_list, depth=0):
    
    if not point_list:
        return None

    k = len(point_list[0])
    axis = depth % k
    
    point_list.sort(key=lambda point: point[axis])
    median = len(point_list) // 2

    node = Node()
    node.location = point_list[median]
    node.left_child = kdtree(point_list[:median], depth + 1)
    node.right_child = kdtree(point_list[median + 1:], depth + 1)
    
    return node


    
