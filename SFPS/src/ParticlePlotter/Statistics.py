import argparse
import ast
import math
import matplotlib.pyplot as plt


def read_txt(txt_path):
    coordinates = []
    with open(txt_path, 'r', encoding='utf-8') as file:
        for line in file:
            coordinates.append(ast.literal_eval(line))
    return coordinates


def create_statistics_graph(coordinates):
    print("Calculating distances...")
    try:
        distance = []
        total_iterations = len(coordinates) * (len(coordinates) - 1) // 2

        for i, coord_i in enumerate(coordinates):
            for j, coord_j in enumerate(coordinates[i+1:], i+1):
                distance_ij = math.sqrt(
                    (coord_i[0] - coord_j[0])**2 +
                    (coord_i[1] - coord_j[1])**2 +
                    (coord_i[2] - coord_j[2])**2)
                distance.append(distance_ij)

                progress = len(distance) / total_iterations * 100
                print(f"Progress: {progress:.2f}%")

        print("Creating histogram...")
        # Create a histogram of the distances
        plt.hist(distance, bins=100)
        plt.xlabel('Distance')
        plt.ylabel('Frequency')
        plt.title('Distance between Particles')
        plt.show()

    except KeyboardInterrupt:
        print("Program interrupted. Exiting gracefully.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Create a statistics graph from a text file.")
    parser.add_argument(
        "txtpath", help="Path to the text file containing particle positions.")

    args = parser.parse_args()
    coordinates = read_txt(args.txtpath)

    if coordinates:
        create_statistics_graph(coordinates)
    else:
        print("No valid coordinates found in the text file.")

# The statics graph is very GPU intensive.
# It is recommended to run this script on this Collab notebook
