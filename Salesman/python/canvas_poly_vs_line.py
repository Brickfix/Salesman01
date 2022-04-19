from cProfile import label
import numpy as np
from tkinter import Tk, Canvas
import time
import matplotlib.pyplot as plt

if __name__ == "__main__":

    point_sizes = [10, 100, 1000, 10000, 100000]
    lines_times = []
    poly_times = []
    
    window = Tk()
    
    for n_points in point_sizes:
        pseudo_points = np.arange(n_points)
        indizes = np.arange(int(n_points/2))
        canvas = Canvas(window)
    
        t_start = time.time()
    
        # lines
        start_point = pseudo_points[indizes[0]], pseudo_points[indizes[0]+1]
        for index in indizes[1:]:
            next_point = pseudo_points[2*index], pseudo_points[2*index+1]
            canvas.create_line((start_point[0], start_point[1], next_point[0], next_point[1]))
            start_point = next_point
        canvas.create_line((start_point[0], start_point[1], pseudo_points[0], pseudo_points[1]))
    
        t_lines = time.time()
    
        # polygon
        canvas.create_polygon(
            *[(pseudo_points[2*index], pseudo_points[2*index+1]) 
            for index in indizes],
            fill="white",
            outline="red"
        )
        t_poly = time.time()
    
        # print(f"{n_points}:")
        # print("  Draw lines:", t_lines-t_start)
        # print("  Draw polygons:", t_poly-t_lines)
        # print("  Difference:", t_poly-t_lines-(t_lines-t_start))
        lines_times.append(t_lines-t_start)
        poly_times.append( t_poly-t_lines)
    
        canvas.destroy()
    
    
    window.destroy()
    
    fig, ax = plt.subplots()
    
    ax1 = plt.subplot(121)
    line0 = ax1.plot(point_sizes, lines_times, 'r', label='lines')
    marker0 = ax1.plot(point_sizes, lines_times, 'rx')
    line1 = ax1.plot(point_sizes, poly_times, 'b', label='poly')
    marker1 = ax1.plot(point_sizes, poly_times, 'bx')
    plt.xlabel('points')
    plt.ylabel('time')
    
    ax2 = plt.subplot(122, sharey=ax1,)
    line0 = ax2.plot(point_sizes, lines_times, 'r')
    marker0 = ax2.plot(point_sizes, lines_times, 'rx')
    line1 = ax2.plot(point_sizes, poly_times, 'b')
    marker1 = ax2.plot(point_sizes, poly_times, 'bx')
    
    fig.legend()
    
    plt.suptitle('Lines vs Poly')
    plt.xscale('symlog')
    
    plt.show()