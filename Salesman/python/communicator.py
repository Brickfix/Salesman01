from subprocess import Popen, PIPE
import os
from typing import Tuple, Sequence, Union


def command_list(
                new_points: Union[int, None],
                input_points: list,
                img_size: Union[int, None],
                seed: Union[str, None],
                mode: Union[int, None]) -> Tuple[list, bool, int]:
    '''
    creates the command list
    faciliates testing
    
    Parameters
    -----------
    new_points: int or None
        Number of new points, following "--points" command.
        if None, no command will be created
    
    img_size: int or None
        Size of the image, following the "--size command
        if None, no command will be created
    
    input_points: list
        x and y coordinates of points to input.
        if list is empty, no command will be created.
        Else, "--input-points" followed by half the len() of the list

    seed: str
        seed to initialize random number generation

    mode: int
        Mode to execute .exe in

    Returns
    --------
    commands: list
        list containing the commands for executable
    
    run_cin_loop: bool
        if a loop to input points must be run

    n_points_input: int
        Number of input points, len(input_points)/2
    '''
    commands = []
    run_cin_loop = False
    n_points_input = 0

    if new_points is not None:
        # generate random
        if new_points < 0:
            raise RuntimeError(f"generated points must be positive, was {new_points}")
        commands.append("--points")
        commands.append(str(new_points))
    
    if len(input_points) > 0:

        n_points_input_float = len(input_points)/2
        # check if input is even
        # as input must contain x and y coords
        if(n_points_input_float%1 != 0):
            raise RuntimeError(f"length of inputs_points must be even, was {n_points_input_float}")
        
        n_points_input = int(n_points_input_float)
        
        commands.append("--inputPoints")
        commands.append(str(n_points_input))
        # set to true to run loop
        run_cin_loop = True

    # check if total is enough points for path finding algorithm (4 or more)
    enough_points = n_points_input + new_points >= 4 if new_points is not None else n_points_input >= 4
    if not enough_points:
        if not (new_points is None and len(input_points)==0):
            raise RuntimeError(
                f"Input requires a total of 4 or more points, provided {n_points_input} plus {new_points} to generate")


    if img_size is not None:
        if img_size <= 0:
            raise RuntimeError(f"img_size must be greater then 0, was {img_size}")
        commands.append("--size")
        commands.append(str(img_size))
    
    if seed is not None:
        commands.append("--seed")
        commands.append(seed)
    
    if mode is not None:
        commands.append("--mode")
        commands.append(str(mode))

    return commands, run_cin_loop, n_points_input

def convert_output(output, new_points, n_points_input):
    '''
    converts the output list to usefull output parameters
    
    Parameters
    -----------
    output: list
        [out.strip() for out in sales_call.stdout.readlines()]
    new_points: int
        number of new points generated
    n_points_intput: int
        number of already given points

    Returns
    --------
    time_total: float
        Time to run entire path finding algorithm
    time_best: float
        time it took to find best path
    min_distance: float
        length of minimum distance
    indexes: list
        list containing the point indexes in shortest path order
    output_points: list
        list cotaining the point coordinates
    '''

    if(len(output) == 0):
        raise RuntimeError("No Output from exe")

    time_total, time_best, min_distance = float(output[-1]), float(output[-2]), float(output[-3])

    total_points = new_points + n_points_input

    indexes = [int(out) for out in output[:total_points]]
    output_points = [int(out) for out in output[total_points:total_points+2*total_points]]

    return time_total, time_best, min_distance, indexes, output_points

def run_salesman_exe(
            new_points: Union[int, None],
            img_size: Union[int, None] = None,
            input_points: list = [],
            seed: str = "711",
            mode: int = 0,
            exe_name: str = "Salesman01.exe",) -> Tuple[
                                                float,
                                                float,
                                                float,
                                                Sequence[int],
                                                Sequence[int]]:
    '''
    runs the Salesman01.exe in a subprocess and returns the results
    
    Parameters
    -----------
    commands: list
        list of strings containing the desired commands
    exe_name: str
        name of the executable for path calculations

    Returns
    --------
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

    real_path = os.path.realpath(exe_name)
    commands, run_cin_loop, n_points_input = command_list(new_points, input_points, img_size, seed, mode)

    sales_call = Popen([real_path, *commands], stdout=PIPE, stdin = PIPE)

    # input points given to exe, read in via std::cin
    if run_cin_loop:
        for p in input_points:
            sales_call.stdin.write((str(p)).encode('UTF-8'))
            sales_call.stdin.write(b"\n")

        sales_call.stdin.flush()

    # output given from exe via std::cout
    output = [out.strip() for out in sales_call.stdout.readlines()]

    return convert_output(output, new_points, n_points_input)
