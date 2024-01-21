import argparse
import ast
import matplotlib.pyplot as plt


def read_txt(txt_path):
    coordinates = []
    with open(txt_path, 'r', encoding='utf-8') as file:
        for line in file:
            coordinates.append(ast.literal_eval(line))
    return coordinates


def create_scatter_plot(coordinates):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    x, y, z = zip(*coordinates)

    ax.scatter(x, y, z)
    ax.set_xlabel('X-axis')
    ax.set_ylabel('Y-axis')
    ax.set_zlabel('Z-axis')

    plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Create a 3D scatter plot from a text file.")
    parser.add_argument(
        "txtpath", help="Path to the text file containing particle positions.")

    args = parser.parse_args()
    coordinates = read_txt(args.txtpath)

    if coordinates:
        create_scatter_plot(coordinates)
    else:
        print("No valid coordinates found in the text file.")
