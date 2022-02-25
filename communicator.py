from subprocess import Popen, PIPE
import os
from typing import Tuple, Sequence, Union


def run_salesman_exe(
            points: Union[int, None],
            img_size: Union[int, None] = None,
            input_points: list = [],
            exe_name: str = "Salesman01.exe",) -> Tuple[
                                                float,
                                                float,
                                                float,
                                                Sequence[int],
                                                Sequence[int]]:
    '''
    runs the Salesman01.exe in a subprocess and returns the results
    
    Parameters
    ----------
    points: int or None
        Set number of points to randomly generate
    img_size: int or None
        Set size of Image
    input_points: list
        List containing points. Must be even amount of entries representing x and y coordinates
    exe_name: str
        name of the executable for path calculations

    Returns
    -------
    time_total: float
        Time required to run path finding algorithm
    time_for_best: float
        Time required to find best solution
    min_distance: float
        Minimum distance along best path
    indizes: Sequenz[int]
        Indizes of points of best path
    points: Sequenz[int]
        Points used for calculation
    '''

    stdin_loop = False

    real_path = os.path.realpath(exe_name)

    if points:
        # generate random
        commands = ["--points", str(points)]
    else:
        # use input
        points = len(input_points)+points if points else int(len(input_points)/2)
        commands = ["--inputPoints", str(points)]
        stdin_loop = True

    if img_size:
        commands.append("--size")
        commands.append(str(img_size))

    sales_call = Popen([real_path, *commands], stdout=PIPE, stdin = PIPE)

    if stdin_loop:
        for p in input_points:
            sales_call.stdin.write((str(p)).encode('UTF-8'))
            sales_call.stdin.write(b"\n")

        sales_call.stdin.flush()


    output = [out.strip() for out in sales_call.stdout.readlines()]

    time_total, time_for_best, min_distance = float(output[-1]), float(output[-2]), float(output[-3])

    indexes = [int(out) for out in output[:points]]
    points = [int(out) for out in output[points:points+2*points]]

    return time_total, time_for_best, min_distance, indexes, points
