import re
import matplotlib.pyplot as plt

# Path del log file
conc_log_path = "tools/minicom.log"
seq_log_path = "tools/minicom_seq.log"

# Lista per salvare i valori di data
time_values = []

def main():
    plot_conc_total()
    plot_conc_first()
    plot_seq()

def plot_conc_total():
    counter = 0
    data_values = []

    with open(conc_log_path, "r") as file:
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
    plt.figure()
    plt.plot(data_values, marker='o', color='green')
    plt.title("Shared Resource Over Time")
    plt.xlabel("Time")
    plt.ylabel("Data")
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("tools/data_plot.png")

def plot_conc_first():
    first_segment = []

    with open(conc_log_path, "r") as file:
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


def plot_seq():
    current_time = 0
    data_values = []
    time_values = []
    

    with open(seq_log_path, "r") as file:
        lastdata = 0
        cnt = 0

        for line in file:
            # Cerca data
            matchInc = re.search(r"Incrementer Task: Data = (\d+)", line)
            matchRes = re.search(r"Resetter Task: Data reset to (\d+)", line)
            if (matchInc):
                data = int(matchInc.group(1))
                lastdata = data
                data_values.append(data)
                time_values.append(current_time)
                current_time += 1  # simula il tempo (ogni incremento = 1 unità)
            
            if (matchRes):
                data = int(matchRes.group(1))
                lastdata = data
                data_values.append(data)
                time_values.append(current_time)
                current_time += 1  # simula il tempo (ogni incremento = 1 unità)

            # Stop quando inizia il fault
            if "Simulating random fault" in line:
                data_values.append(lastdata)
                time_values.append(current_time)
                current_time += 1
                cnt += 1

            # the following instructions are for plotting purpose only
            #if cnt == 5:
            #    break

    plt.figure()
    plt.plot(time_values, data_values, marker='o')
    plt.title("Data evolution in SEQ mode (until system freeze)")
    plt.xlabel("Simulated Time (increment events)")
    plt.ylabel("Shared Resource Value (data)")
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("tools/seq_data_plot.png")
    

if __name__ == "__main__":
    main()