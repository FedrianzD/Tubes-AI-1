import tkinter as tk
from tkinter import ttk, filedialog
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import time

# Class to represent each iteration with attributes for different algorithms
class Iteration:
    def __init__(self, iteration, value, cube, probability=None, switches=None):
        self.iteration = iteration
        self.value = value
        self.cube = cube
        self.probability = probability  # Only used for SA
        self.switches = switches  # Used for all algorithms

# Function to parse the input file and create Iteration objects
def parse_input_file(filename):
    iterations = []
    algorithm_type = None
    initial_cube = []
    execution_time = None  # Placeholder for the execution time

    with open(filename, 'r') as file:
        lines = file.readlines()
        i = 0

        # Determine the type of the algorithm
        algorithm_type = lines[i].strip()
        i += 1

        initial_state_num = int(lines[i].strip())
        i += 1
        initial_state_value = int(lines[i].strip())
        i += 1

        # Read the initial state cube and convert the 1D array to a 3D cube
        initial_cube_1d = list(map(int, lines[i].strip().split()))
        size = int(round(len(initial_cube_1d) ** (1/3)))
        initial_cube = [[[initial_cube_1d[x + y * size + z * size * size] for x in range(size)] for y in range(size)] for z in range(size)]
        i += 1

        # Parse iterations based on the type
        if algorithm_type == "HC":
            iterations.append(Iteration(0, initial_state_value, initial_cube))
            while i < len(lines):
                if lines[i].strip().startswith("Execution Time:"):
                    execution_time = float(lines[i].split(":")[1].strip())
                    break  # Stop reading after finding the execution time
                
                iteration_num = int(lines[i].strip())
                i += 1

                switches = list(map(int, lines[i].strip().split()))
                i += 1

                new_cube = [[[iterations[-1].cube[x][y][z] for z in range(size)] for y in range(size)] for x in range(size)]
                new_cube[switches[0]][switches[1]][switches[2]], new_cube[switches[3]][switches[4]][switches[5]] = (
                    new_cube[switches[3]][switches[4]][switches[5]], new_cube[switches[0]][switches[1]][switches[2]]
                )

                state_value = int(lines[i].strip())
                i += 1
                iterations.append(Iteration(iteration_num, state_value, new_cube, switches=switches))

        elif algorithm_type == "SA":
            iterations.append(Iteration(0, initial_state_value, initial_cube, probability=1))
            while i < len(lines):
                if lines[i].strip().startswith("Execution Time:"):
                    execution_time = float(lines[i].split(":")[1].strip())
                    break

                iteration_num = int(lines[i].strip())
                i += 1
                probability = float(lines[i].strip())
                i += 1
                switches = list(map(int, lines[i].strip().split()))
                i += 1

                if (switches[0] == -1):
                    new_cube = [[[iterations[-1].cube[x][y][z] for z in range(size)] for y in range(size)] for x in range(size)]
                    state_value = int(lines[i].strip())
                    iterations.append(Iteration(iteration_num, state_value, new_cube, probability=probability))
                else:
                    new_cube = [[[iterations[-1].cube[x][y][z] for z in range(size)] for y in range(size)] for x in range(size)]
                    print(iteration_num)
                    new_cube[switches[0]][switches[1]][switches[2]], new_cube[switches[3]][switches[4]][switches[5]] = (
                        new_cube[switches[3]][switches[4]][switches[5]], new_cube[switches[0]][switches[1]][switches[2]]
                    )

                    state_value = int(lines[i].strip())

                    iterations.append(Iteration(iteration_num, state_value, new_cube, probability=probability, switches=switches))
                i += 1

        elif algorithm_type == "GA":
            iterations.append(Iteration(0, initial_state_value, initial_cube))
            num_generations = int(lines[i].strip())
            i += 1
            generations = []
            for _ in range(num_generations):
                generation_cube_1d = list(map(int, lines[i].strip().split()))
                generation_cube = [[[generation_cube_1d[x + y * size + z * size * size] for x in range(size)] for y in range(size)] for z in range(size)]
                generations.append(generation_cube)
                i += 1

            while i < len(lines):
                if lines[i].strip().startswith("Execution Time:"):
                    execution_time = float(lines[i].split(":")[1].strip())
                    break

                iteration_num = int(lines[i].strip())
                i += 1
                state_value = int(lines[i].strip())
                i += 1
                switches = list(map(int, lines[i].strip().split()))
                i += 1

                new_cube = [[[iterations[-1].cube[x][y][z] for z in range(size)] for y in range(size)] for x in range(size)]
                new_cube[switches[0]][switches[1]][switches[2]], new_cube[switches[3]][switches[4]][switches[5]] = (
                    new_cube[switches[3]][switches[4]][switches[5]], new_cube[switches[0]][switches[1]][switches[2]]
                )
                iterations.append(Iteration(iteration_num, state_value, new_cube, switches=switches))

    return algorithm_type, iterations, execution_time


def visualize_cube(ax, iteration_obj):
    ax.clear()
    size = len(iteration_obj.cube)
    switched_nodes = iteration_obj.switches  # List of switched nodes, if any

    for x in range(size):
        for y in range(size):
            for z in range(size):
                value = iteration_obj.cube[x][y][z]

                # Check if the current (x, y, z) coordinate is part of the switched nodes
                if switched_nodes and [x, y, z] in [switched_nodes[:3], switched_nodes[3:]]:
                    color = 'red'  # Use red for switched nodes
                else:
                    color = 'black'  # Default color for other nodes

                # Add labels for each cube cell
                ax.text(x + 0.5, y + 0.5, z + 0.5, str(value), color=color,
                        ha='center', va='center', fontsize=8)

    ax.set_xlim([0, size])
    ax.set_ylim([0, size])
    ax.set_zlim([0, size])
    ax.set_axis_off()
    ax.view_init(elev=30, azim=30)
    ax.set_title('3D Magic Cube Visualization')


def visualize_value_plot(ax, iterations, plot_type="state"):
    ax.clear()
    x = [iteration.iteration for iteration in iterations]
    if plot_type == "state":
        y = [iteration.value for iteration in iterations]
        ax.set_ylabel('State Value')
    elif plot_type == "probability" and hasattr(iterations[0], 'probability'):
        y = [iteration.probability for iteration in iterations]
        ax.set_ylabel('Probability')

    ax.plot(x, y, marker='o', color='b', linestyle='-', linewidth=2)
    ax.set_title(f'Iteration vs. {plot_type.capitalize()} (2D Plot)')
    ax.set_xlabel('Iteration')
    ax.grid(True)

def update_plot(index, iterations, ax, canvas, iteration_label, value_label, fig, view_mode="cube", algorithm_type="HC", plot_type="state"):
    fig.clf()  # Clear the figure to reset the axes
    if view_mode == "cube":
        ax = fig.add_subplot(111, projection='3d')  # Create a 3D axis for the cube
        visualize_cube(ax, iterations[index])
        iteration_label.config(text=f"Iteration: {iterations[index].iteration}")
        
        # Display different state details based on the algorithm type
        if algorithm_type == "SA":
            value_label.config(text=f"State Value: {iterations[index].value}\nProbability: {iterations[index].probability:.4f}")
        elif algorithm_type == "GA":
            value_label.config(text=f"State Value: {iterations[index].value}\nState Transitions: {len(iterations[index].cube)}")
        else:  # HC or other algorithms
            value_label.config(text=f"State Value: {iterations[index].value}")

    elif view_mode == "plot":
        ax = fig.add_subplot(111)  # Create a 2D axis for the value plot
        visualize_value_plot(ax, iterations, plot_type=plot_type)
        iteration_label.config(text=f"Iteration vs. {plot_type.capitalize()} ({algorithm_type})")
        value_label.config(text="")  # Clear state value label for the plot view

    canvas.draw()
    return ax  # Return the updated axis object

def main():
    # Initialize variables
    global algorithm_type
    algorithm_type = "N/A"  # Placeholder for the type of algorithm

    def load_file():
        filename = filedialog.askopenfilename(filetypes=[("Text files", "*.txt"), ("All files", "*.*")])
        if filename:
            nonlocal iterations
            global algorithm_type

            algorithm_type, iterations, execution_time = parse_input_file(filename)
            
            if iterations:
                current_index[0] = 0
                update_plot(current_index[0], iterations, ax, canvas, iteration_label, value_label, fig, view_mode=current_view, algorithm_type=algorithm_type)
                progress_var.set(0)
                progress_bar.config(to=len(iterations) - 1)
                playback_speed.set("1x")
                play_pause_button.config(text="Play")
                play_status[0] = False

        # Update algorithm type label after loading the file
        algorithm_label.config(text=f"Algorithm: {algorithm_type}")
        execution_time_label.config(text=f"Execution Time: {execution_time}")

    def play_pause():
        if play_status[0]:
            play_pause_button.config(text="Play")
            play_status[0] = False
        else:
            play_pause_button.config(text="Pause")
            play_status[0] = True
            play_iterations()

    def play_iterations():
        while play_status[0] and current_index[0] < len(iterations) - 1:
            current_index[0] += 1
            progress_var.set(current_index[0])
            update_plot(current_index[0], iterations, ax, canvas, iteration_label, value_label, fig, algorithm_type=algorithm_type)
            root.update()
            time.sleep(playback_speed_map[playback_speed.get()])

            if current_index[0] == len(iterations) - 1:
                play_pause_button.config(text="Play")
                play_status[0] = False

    def on_progress_change(value):
        current_index[0] = int(float(value))  # Convert the value to float first, then to int
        update_plot(current_index[0], iterations, ax, canvas, iteration_label, value_label, fig, algorithm_type=algorithm_type)

    def toggle_view():
        nonlocal current_view, ax, plot_type_dropdown
        if current_view == "cube":
            current_view = "plot"
            toggle_view_button.config(text="Show Cube Visualization")
            control_frame.pack_forget()  # Hide control_frame
            
            # Show the plot type dropdown for SA algorithm
            if algorithm_type == "SA":
                plot_type_dropdown.pack(side=tk.TOP, pady=5)
        else:
            current_view = "cube"
            toggle_view_button.config(text="Show Value Plot")
            control_frame.pack(side=tk.BOTTOM, fill=tk.X, padx=5, pady=5)  # Show control_frame

            # Hide the plot type dropdown for SA
            if plot_type_dropdown:
                plot_type_dropdown.pack_forget()

        ax = update_plot(current_index[0], iterations, ax, canvas, iteration_label, value_label, fig, view_mode=current_view, algorithm_type=algorithm_type, plot_type=plot_type_var.get())

    # Create the main window
    root = tk.Tk()
    root.title("Magic Cube Visualizer")
    root.geometry("800x600")
    root.configure(bg="#f5f5f5")

    plot_type_var = tk.StringVar(value="state")
    plot_type_dropdown = None

    # Create a custom theme
    style = ttk.Style()
    style.theme_use('clam')
    
    style.configure('TButton', background='#4CAF50', foreground='white', font=('Arial', 12, 'bold'))
    style.map('TButton', background=[('active', '#45a049')], foreground=[('active', 'white')])
    style.configure('TLabel', background="#eaeaea", font=('Arial', 12))

    # Create a frame for the plot
    plot_frame = tk.Frame(root, bg="#ffffff", bd=2, relief=tk.SUNKEN)
    plot_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=10, pady=10)

    # Create a matplotlib figure and axis
    fig = plt.figure(figsize=(5, 4))
    ax = fig.add_subplot(111, projection='3d')

    # Embed the matplotlib figure in the Tkinter window
    canvas = FigureCanvasTkAgg(fig, master=plot_frame)
    canvas_widget = canvas.get_tk_widget()
    canvas_widget.pack(side=tk.TOP, fill=tk.BOTH, expand=True)

    # Create a control frame
    control_frame = tk.Frame(plot_frame, bg="#eaeaea")
    control_frame.pack(side=tk.BOTTOM, fill=tk.X, padx=5, pady=5)

    # Create play/pause button
    play_pause_button = ttk.Button(control_frame, text="Play", command=play_pause)
    play_pause_button.pack(side=tk.LEFT, padx=5, pady=5)

    # Create a progress bar
    progress_var = tk.IntVar()
    progress_bar = ttk.Scale(control_frame, from_=0, to=0, variable=progress_var, orient="horizontal",
                             command=lambda v: on_progress_change(v))
    progress_bar.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=5)

    # Create speed selection dropdown
    playback_speed = tk.StringVar(value="1x")
    playback_speed_map = {"0.5x": 1.0, "1x": 0.5, "2x": 0.25}
    speed_dropdown = ttk.OptionMenu(control_frame, playback_speed, "1x", *playback_speed_map.keys())
    speed_dropdown.pack(side=tk.LEFT, padx=5, pady=5)

    # Create an info frame
    info_frame = tk.Frame(root, bg="#eaeaea", width=250)
    info_frame.pack(side=tk.RIGHT, fill=tk.Y, padx=10, pady=10)
    info_frame.pack_propagate(False)  # Prevent the frame from resizing to fit its content

    # Update the info frame to include the algorithm type display
    algorithm_label = ttk.Label(info_frame, text="Algorithm: N/A", anchor="center", justify="center", font=('Arial', 16, 'bold'))
    algorithm_label.pack(pady=10, padx=10, fill=tk.X)

    # Update the info frame to include the execution time type display
    execution_time_label = ttk.Label(info_frame, text="Execution Time: N/A", anchor="center", justify="center", font=('Arial', 16, 'bold'))
    execution_time_label.pack(pady=10, padx=10, fill=tk.X)

    # Create labels for iteration and state value with updated styling
    iteration_label = ttk.Label(info_frame, text="Iteration: N/A", anchor="center", justify="center", 
                                font=('Arial', 16, 'bold'))
    iteration_label.pack(pady=20, padx=10, fill=tk.X)

    value_label = ttk.Label(info_frame, text="State Value: N/A", anchor="center", justify="center", 
                            font=('Arial', 16, 'bold'))
    value_label.pack(pady=20, padx=10, fill=tk.X)

    # Create a load file button
    load_button = ttk.Button(info_frame, text="Load File", command=load_file)
    load_button.pack(pady=20, padx=10)

    # Create a toggle view button and place it under the load file button
    toggle_view_button = ttk.Button(info_frame, text="Show Value Plot", command=toggle_view)
    toggle_view_button.pack(pady=10, padx=10)

    plot_type_dropdown = ttk.OptionMenu(info_frame, plot_type_var, "state", "state", "probability", command=lambda _: update_plot(current_index[0], iterations, ax, canvas, iteration_label, value_label, fig, view_mode="plot", algorithm_type=algorithm_type, plot_type=plot_type_var.get()))
    plot_type_dropdown.pack_forget()

    # Initialize variables
    iterations = []
    current_index = [0]
    play_status = [False]
    current_view = "cube"  # Track the current view (cube or plot)

    # Run the Tkinter event loop
    root.mainloop()

if __name__ == "__main__":
    main()


