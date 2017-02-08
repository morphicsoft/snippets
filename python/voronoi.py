#!/usr/bin/env python

import numpy as np
from numpy import random
from scipy.spatial import KDTree
import matplotlib.pyplot as plt
from PIL import Image

n_points = 50000
n_regions = 30
width = 1280
height = 960

input_x = np.random.randint(1, width, n_points)
input_y = np.random.randint(1, height, n_points)

output_x = np.random.randint(1, width, n_regions)
output_y = np.random.randint(1, height, n_regions)

region_colours = [ '#cc0000', '#43c4ef', '#b9d7d9', '#73623d',
                   '#0000ff', '#df692f', '#00ff00', '#ee82ee',
                   '#008080', '#65a7d2' ]

def colour(num):

    colours = []
    for i in range(num):
        r = np.random.randint(0, 255)
        g = np.random.randint(0, 255)
        b = np.random.randint(0, 255)
        colours.append((r,g,b))

    for c in colours:
        yield c

def squared_distance(p1, p2):

    x_dist = p2[0] - p1[0]
    y_dist = p2[1] - p1[1]

    return x_dist * x_dist + y_dist * y_dist

'''
brute-force algorithm to identify closest output point
to each input point
(doesn't yet do the voronoi tessellation)
'''
def voronoi(input, output):

    points = {}
    for ip in input:

        closest = float("inf")
        for op in output:
            dist = squared_distance(ip, op)
            if dist < closest:
                closest = dist
                closest_point = op

        try:
            points[closest_point].append(ip)
        except KeyError:
            points[closest_point] = [ip]

    return points

def circle(image, centre_x, centre_y, radius):

    import math

    cf = math.pi/180

    image.putpixel((centre_x, centre_y), (0,0,0))
    for r in range(1,radius):
        for theta in range(1,360,2):
            
            x = int(r * math.cos(cf * theta)) + centre_x
            y = int(r * math.sin(cf * theta)) + centre_y

            if x >= 0 and y >= 0 and x < width and y < height:
                image.putpixel((x, y), (0,0,0))
    

def display(vor):
    
    c = colour(len(vor))
    plt.plot(input_x, input_y, 'kx', linewidth=10)
    for k, v in vor.items():
        plt.plot([x[0] for x in v], [y[1] for y in v], marker='o', color=c.next())

    plt.axis([0, 200, 0, 200])
    plt.show()

'''
This simply draws voronoi regions straight onto a bitmap image.
Voronoi tessellation is not performed.
'''
def voronoi2(w, h, cells):

    image = Image.new("RGB", (w, h))
    putpixel = image.putpixel

    c = colour(len(cells))

    colours = {}
    for cell in cells:
        colours[cell] = c.next()

    for y in range(h):
        for x in range(w):

            closest = float("inf")

            for cell in cells:
                dist = squared_distance((x,y), cell)

                if dist < closest:
                    closest = dist
                    closest_point = cell

            putpixel((x, y), colours[closest_point])


    for cell in cells:
        circle(image, cell[0], cell[1], 4)
        
    image.save("output.png", "PNG")
    image.show()



#vor = voronoi(zip(input_x, input_y), zip(output_x, output_y))
voronoi2(width, height, zip(output_x, output_y))

