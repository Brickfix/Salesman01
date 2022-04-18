from tkinter import *

from typing import Sequence
import numpy as np

from communicator import run_salesman_exe

class SalesGUI:

    canvas: Canvas
    window: Tk
    radius: int
    indizes: Sequence
    points: list
    n_points: int
    time_total: float
    time_best: float
    min_distance: float

    #### constant strings to prevent typos
    # Mode of point selection/generation
    POINT_MODE_CUSTOM="custom"
    POINT_MODE_RANDOM="random"
    # add or regen random points
    RANDOM_MODE_ADD="add"
    RANDOM_MODE_REGEN="regen"
    # add or remove points on canvas
    CUSTOM_MODE_ADD="addPoint"
    CUSTOM_MODE_REMOVE="removePoint"

    # options                                                           CAREFULL: GeneratePoints must be last!
    option_list = ["Permutate All", "Permutate Half", "ClosestNextPoint", "NoCross", "GeneratePoints"]

    def __init__(
                self,
                n_points: int,
                canvas_size: int,
                radius: int = 10,
                title: str = "SalesGUI"):
        '''
        Initializes the GUI
        
        canvas_size: int
            size of the Canvas in pixel
        
        
        '''

        # parameters
        self.radius = radius
        self.n_points = n_points
        self.canvas_size = canvas_size

        # run salesman exe once
        self.run_default()

        # build the actual gui
        self.window = Tk()
        self.window.title(title)
        # self.window.columnconfigure(0, weight=1)
        # self.window.rowconfigure(0, weight=1)

        self.canvas_desc=Label(self.window, text="Display of Results")
        self.canvas_desc.grid(column=0, row=0)

        self.option_label = Label(self.window, text="Options")
        self.option_label.grid(column=1, row=0)

        # Frames containing options
        containFrame = Frame(self.window)
        containFrame.grid(column=1, row=1, sticky=NW)

        # Variable tracking mode
        self.mode_var = StringVar()
        self.create_frame_custom(containFrame)
        self.create_frame_random(containFrame)
        
        # Select program to exectue
        self.create_frame_program(containFrame)

        # Information about run
        self.create_frame_info(containFrame)

        # create canvas once
        self.clear_canvas()

        exit_button = Button(self.window, text="Exit", command=self.window.destroy)
        exit_button.grid(column=0, row=4)

        run_button = Button(self.window, text="Run", command=self.run_custom)
        run_button.grid(column=1, row=4)

        # draw_button = Button(self.window, text="Draw", command=self.draw_path_and_points)
        # draw_button.grid(column=1, row=0)

        # add_button = Button(self.window, text="Add Point", command=self.add_point)
        # add_button.grid(column=1, row=0)
        

        # start up
        self.draw_path_and_points()

    def create_frame_custom(self, master):
        '''
        Creates a Frame with widgets for custom points
        '''
        self.custom_points_frame = Frame(master, bd=2, relief=GROOVE, padx=5, pady=5)
        self.custom_points_frame.grid(column=0, row=0, sticky=[NW, E], pady=5)

        custom_points_select = Radiobutton(self.custom_points_frame,
                            variable=self.mode_var, value=self.POINT_MODE_CUSTOM, text="Custom Points",
                            command=self.points_mode_change)
        custom_points_select.grid(column=0, row=0, columnspan=2)
        self.mode_var.set(self.POINT_MODE_CUSTOM)

        # bools for buttons
        self.add_point_on = False
        self.remove_point_on = False

        # button to add point
        custom_add_button = Button(self.custom_points_frame, command=self.add_button_click)
        self.custom_add_button_text = StringVar(custom_add_button, "Add")
        custom_add_button.configure(textvariable=self.custom_add_button_text)
        custom_add_button.grid(column=0, row=1)

        # button to remove point
        custom_remove_button = Button(self.custom_points_frame, command=self.remove_button_click)
        self.custom_remove_button_text = StringVar(custom_remove_button, "Remove")
        custom_remove_button.configure(textvariable=self.custom_remove_button_text)
        custom_remove_button.grid(column=1, row=1)

    def create_frame_random(self, master):
        '''
        Creates a Frame with windgets for random points
        '''
        self.random_points_frame = Frame(master, bd=2, relief=GROOVE, padx=5, pady=5)
        self.random_points_frame.grid(column=0, row=1, sticky=[NW, E], pady=5)

        random_points_select = Radiobutton(self.random_points_frame,
                                variable=self.mode_var, value=self.POINT_MODE_RANDOM, text="Random Points",
                                command=self.points_mode_change)
        random_points_select.grid(column=0, row=0)

        random_mode_frame = Frame(self.random_points_frame, padx=10)
        random_mode_frame.grid(column=0, row=1, sticky=W)


        self.random_mode = StringVar()
        add_mode_radio = Radiobutton(random_mode_frame, 
                                variable=self.random_mode, value=self.RANDOM_MODE_ADD, text="Add", takefocus=0,
                                command=self.rand_mode_change)
        add_mode_radio.grid(column=0, row=0, sticky=W)
        regen_mode_radio = Radiobutton(random_mode_frame,
                                variable=self.random_mode, value=self.RANDOM_MODE_REGEN,text="Regenerate", takefocus=0,
                                command=self.rand_mode_change)
        regen_mode_radio.grid(column=0, row=1, sticky=W)
        self.random_mode.set(self.RANDOM_MODE_ADD)

        amount_label = Label(self.random_points_frame, text="Set Amount")
        amount_label.grid(column=0, row=2, sticky=W)
        self.amount_input = Entry(self.random_points_frame)
        self.amount_input.grid(column=1, row=2, sticky=W)

        seed_label = Label(self.random_points_frame, text="Seed")
        seed_label.grid(column=0, row=3, sticky=W)
        self.seed_input = Entry(self.random_points_frame)
        self.seed_input.grid(column=1, row=3, sticky=W)

    def create_frame_program(self, master):
        '''
        creates a frame where the desired program option can be selected
        '''
        
        program_frame = Frame(master, bd=2, relief=GROOVE, padx=5, pady=5)
        program_frame.grid(column=0, row=2, sticky=[NW, E])

        program_label = Label(program_frame, text="Program:")
        program_label.grid(column=0, row=0, sticky=W)
        
        self.option = StringVar()
        self.option.set(self.option_list[0])
        options_menu = OptionMenu(program_frame, self.option, *self.option_list)
        options_menu.grid(column=1, row=0, sticky=W)

    def create_frame_info(self, master):
        '''
        creates the frame containing information about the last run
        '''

        info_frame = Frame(master, bd=2, relief=GROOVE ,padx=5, pady=5)
        info_frame.grid(column=0, row=3, sticky=[NW, E])

        distance_info_label = Label(info_frame, text="Minimum Distance")
        distance_info_label.grid(column=0, row=0, sticky=W)

        distance_label = Label(info_frame)
        self.distance_str = StringVar(distance_label, str(self.min_distance))
        distance_label.configure(textvariable=self.distance_str)
        distance_label.grid(column=1, row=0, sticky=W)

        total_time_info_label = Label(info_frame, text="Total time for calculation:")
        total_time_info_label.grid(column=0, row=1, sticky=W)

        total_time_label = Label(info_frame)
        self.total_time_str = StringVar(total_time_label, str(self.time_total))
        total_time_label.configure(textvariable=self.total_time_str)
        total_time_label.grid(column=1, row=1, sticky=W)

        best_time_info_label = Label(info_frame, text="Time until best:")
        best_time_info_label.grid(column=0, row=2, sticky=W)

        best_time_label = Label(info_frame)
        self.best_time_str = StringVar(best_time_label, str(self.time_best))
        best_time_label.configure(textvariable=self.best_time_str)
        best_time_label.grid(column=1, row=2, sticky=W)

        warning_info_label = Label(info_frame, text="WARNING:")
        warning_info_label.grid(column=0, row=3, sticky=W)

        warning_label = Label(info_frame)
        self.warning_str = StringVar(warning_label, "")
        warning_label.configure(textvariable=self.warning_str)
        warning_label.grid(column=1, row=3, sticky=W)

    def rand_mode_change(self):
        print(self.random_mode.get())
    
    def points_mode_change(self):
        '''
        Gets called when the mode is switched between
        random point mode and custom set points mode
        
        '''
        if self.mode_var.get() == self.POINT_MODE_CUSTOM:
            print(self.mode_var.get())


    def add_button_click(self):
        if self.add_point_on:
            self.custom_add_button_text.set("Add")
            self.add_point_on=False
            self.canvas.bind("<Button-1>", None)
        else:
            self.custom_add_button_text.set("Finish")
            self.add_point_on=True

            self.custom_remove_button_text.set("Remove")
            self.remove_point_on=False

            self.canvas.bind("<Button-1>", self.draw_point)

    def remove_button_click(self):
        if self.remove_point_on:
            self.custom_remove_button_text.set("Remove")
            self.remove_point_on=False
            self.canvas.bind("<Button-1>", None)
        else:
            self.custom_remove_button_text.set("Finish")
            self.remove_point_on=True

            self.custom_add_button_text.set("Add")
            self.add_point_on=False

            self.canvas.bind("<Button-1>", self.remove_point)


    def clear_canvas(self):
        '''
        destroys and rebuilds the canvas as blanck
        
        '''

        try:
            self.canvas.destroy()
        except AttributeError:
            # no canvas attribute -> first time creation
            pass

        self.canvas = Canvas(self.window, background="white", width=self.canvas_size, height=self.canvas_size)
        self.canvas.grid(column=0, row=1, sticky=(W),
                            columnspan=1, rowspan=3)

        if self.remove_point_on:
            self.canvas.bind("<Button-1>", self.remove_point)
        elif self.add_point_on:
            self.canvas.bind("<Button-1>", self.draw_point)

    def draw_point(self, event):
        '''
        draws one new point
        '''
        x_coord, y_coord = event.x, event.y

        self.points.append(x_coord)
        self.points.append(y_coord)

        coordinates = (
            x_coord-self.radius,
            y_coord-self.radius,
            x_coord+self.radius,
            y_coord+self.radius
        )

        self.canvas.create_oval(coordinates, fill="blue")

        self.n_points += 1


    def remove_point(self, event):
        '''
        removes a point
        '''

        x_coords_array = np.array(self.points[::2])
        y_coords_array = np.array(self.points[1::2])
        
        x_mask_smaller = x_coords_array <= event.x+self.radius
        x_mask_larger = x_coords_array >= event.x-self.radius

        x_mask = np.all([x_mask_larger, x_mask_smaller], axis=0)

        y_mask_smaller = y_coords_array <= event.y+self.radius
        y_mask_larger = y_coords_array >= event.y-self.radius

        y_mask = np.all([y_mask_smaller, y_mask_larger], axis=0)

        mask = np.all([x_mask, y_mask], axis=0)

        indizes = np.arange(len(x_coords_array))

        if len(indizes[mask])==0:
            return

        index = indizes[mask][0]

        self.points.pop(2*index)
        self.points.pop(2*index)

        self.clear_canvas()
        self.draw_points()

        self.n_points -=1

    def draw_points(self):
        '''
        draws the points
        '''

        for p_ind in range(int(len(self.points)/2)):
            # oval creates and oval within the bounding box specified by x0, y0, x1, y1
            coordinates = (
                self.points[2*p_ind]-self.radius,
                self.points[2*p_ind+1]-self.radius,
                self.points[2*p_ind]+self.radius,
                self.points[2*p_ind+1]+self.radius
            )
            self.canvas.create_oval(coordinates, fill="blue")

    def draw_path_and_points(self):
        '''
        draws the path and points
        '''
        # start_point = points[indizes[0]], points[indizes[0]+1]
        # for index in indizes[1:]:
        #     next_point = points[2*index], points[2*index+1]
        #     canvas.create_line((start_point[0], start_point[1], next_point[0], next_point[1]))
        #     start_point = next_point
        # canvas.create_line((start_point[0], start_point[1], points[0], points[1]))

        self.canvas.create_polygon(
            *[(self.points[2*index], self.points[2*index+1]) 
            for index in self.indizes],
            fill="white",
            outline="red"
        )

        for p_ind in range(len(self.indizes)):
            # oval creates and oval within the bounding box specified by x0, y0, x1, y1
            coordinates = (
                self.points[2*p_ind]-self.radius,
                self.points[2*p_ind+1]-self.radius,
                self.points[2*p_ind]+self.radius,
                self.points[2*p_ind+1]+self.radius
            )
            self.canvas.create_oval(coordinates, fill="blue")

    def run_custom(self):
        '''
        Runs the salesman exe once on custom point configuration
        '''

        # The way input arguments are generated here could definitly be cleaned up, 
        # but hey it works
        if self.mode_var.get() == self.POINT_MODE_CUSTOM:
            input_args = dict(
                new_points=0,
                img_size=self.canvas_size,
                input_points=self.points
            )
        elif self.mode_var.get() == self.POINT_MODE_RANDOM:
            
            p = int(self.amount_input.get())

            if self.random_mode.get() == self.RANDOM_MODE_ADD:
                # add additional specified points
                input_points = self.points
            elif self.random_mode.get() == self.RANDOM_MODE_REGEN:
                # replace existing points
                input_points = []
            else:
                msg = f"Invalid random mode {self.random_mode.get()}"
                self.warning_str.set(msg)
                raise RuntimeError(msg)
            
            input_args = dict(
                new_points=p,
                img_size=self.canvas_size,
                input_points=input_points
            )
            if len(self.seed_input.get()) > 0:
                input_args['seed'] = self.seed_input.get()

        else:
            msg = f"Invalid mode {self.mode_var.get()}"
            self.warning_str.set(msg)
            raise RuntimeError()
        
        input_args['mode'] = self.option_list.index(self.option.get())

        try:
            self.time_total, self.time_best, self.min_distance, self.indizes, self.points = \
                run_salesman_exe(**input_args)
        except RuntimeError as e:
            self.warning_str.set(e.args[0])
            return


        self.n_points = int(len(self.points)/2)

        self.total_time_str.set(str(self.time_total))
        self.best_time_str.set(str(self.time_best))
        self.distance_str.set(str(self.min_distance))

        self.clear_canvas()
        self.draw_path_and_points()

        self.warning_str.set("")

    def run_default(self):
        '''
        Runs the salesman exe once on random point configuration
        
        '''

        self.time_total, self.time_best, self.min_distance, self.indizes, self.points = \
            run_salesman_exe(self.n_points, self.canvas_size, mode=-1)
        
    def show(self):

        self.window.mainloop()


if __name__ == "__main__":
    gui = SalesGUI(n_points = 10, canvas_size = 500)
    gui.show()