from unittest import TestCase
from communicator import command_list, convert_output

class TestCommunicator(TestCase):

    def test_for_errors(self):
        '''
        Test the communicator functions for correct error response on faulty input
        
        '''
        # Test if too few points is recognized
        input_dict = dict(
            new_points=None,
            input_points=[100, 50, 75, 200],
            img_size=None,
            seed=None
        )
        with self.assertRaises(RuntimeError):
            command_list(**input_dict)

        input_dict = dict(
            new_points=3,
            input_points=[],
            img_size=None,
            seed=None
        )
        with self.assertRaises(RuntimeError):
            command_list(**input_dict)

        # Test if uneven amount of points is recognized
        input_dict = dict(
            new_points=None,
            input_points=[100, 50, 75, 200, 350, 100, 100, 25, 85],
            img_size=None,
            seed=None
        )
        with self.assertRaises(RuntimeError):
            command_list(**input_dict)

        # Test for negative number inputs
        input_dict = dict(
            new_points=-5,
            input_points=[],
            img_size=None,
            seed=None
        )
        with self.assertRaises(RuntimeError):
            command_list(**input_dict)

        input_dict = dict(
            new_points=None,
            input_points=[],
            img_size=-200,
            seed=None
        )
        with self.assertRaises(RuntimeError):
            command_list(**input_dict)

    def test_commands(self):
        '''
        test the command_list() function for correct output
        
        '''

        # check for correct defaults
        input_dict = dict(
            new_points=None,
            input_points=[],
            img_size=None,
            seed=None
        )
        commands, cin_loop, n_points_input = command_list(**input_dict)
        self.assertEqual(len(commands), 0)
        self.assertFalse(cin_loop)
        self.assertEqual(n_points_input, 0)

        # check for correct loop
        input_dict = dict(
            new_points=None,
            input_points=[10, 10, 20, 20, 30, 30, 40, 40, 50, 50],
            img_size=None,
            seed=None
        )
        commands, cin_loop, n_points_input = command_list(**input_dict)
        self.assertEqual(len(commands), 2)
        self.assertTrue(cin_loop)
        self.assertEqual(n_points_input, 5)

        # check for correct combination of commands
        input_dict = dict(
            new_points=4,
            input_points=[10, 10, 20, 20, 30, 30, 40, 40, 50, 50],
            img_size=300,
            seed=1234
        )
        commands, cin_loop, n_points_input = command_list(**input_dict)
        self.assertEqual(len(commands), 8)
        self.assertTrue(cin_loop)
        self.assertEqual(n_points_input, 5)

    def test_output_converter(self):
        '''
        test if the convert_output() function convertes bytes to correct output
        '''

        output_list = [b'0', b'1', b'2', b'3', b'50', b'50', b'100', b'50', b'100', b'100', b'50', b'100', b'200', b'0', b'0']
        new_points = 0
        input_points = 4

        time_total, time_best, min_distance, indexes, output_points = \
            convert_output(output_list, new_points, input_points)
        self.assertEqual(0, time_total)
        self.assertEqual(0, time_best)
        self.assertEqual(200, min_distance)
        self.assertEqual(len(indexes), 4)
        self.assertEqual(len(output_points), 8)
