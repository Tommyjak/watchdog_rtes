import re
import matplotlib.pyplot as plt

# Path del log file
log_path = "tools/minicom.log"

# Lista per salvare i valori di data
data_values = []

def main():
    plot_total()
    plot_first()

def plot_total():
    counter = 0
    with open(log_path, "r") as file:
        for line in file:
            if "reboot" in line.lower():
                counter += 1

            if counter == 5:
                break

            match = re.search(r"Incrementer Task: Incremented data to (\d+)", line)
            if match:
                value = int(match.group(1))
                data_values.append(value)


    # Plot dei dati
    plt.plot(data_values, marker='o', color='green')
    plt.title("Shared Resource Over Time")
    plt.xlabel("Time")
    plt.ylabel("Data")
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("tools/data_plot.png")

def plot_first():
    first_segment = []

    with open(log_path, "r") as file:
        for line in file:
            # Stop if "reboot" is found (case-insensitive)
            if "reboot" in line.lower():
                break

            match = re.search(r"Incrementer Task: Incremented data to (\d+)", line)
            if match:
                value = int(match.group(1))
                first_segment.append(value)

    # Plot dei dati prima del reboot
    plt.figure()
    plt.plot(first_segment, marker='o', color='red')
    plt.title("Shared Resource Before First Reboot")
    plt.xlabel("Time")
    plt.ylabel("Data")
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("tools/first_iteration_plot.png")

if __name__ == "__main__":
    main()