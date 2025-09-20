import serial
import serial.tools.list_ports

def list_devices():
    ports = serial.tools.list_ports.comports()
    return [(p.device, p.description) for p in ports]

def choose_device():
    devices = list_devices()
    if not devices:
        print("\nNo devices found.")
        return None

    print("\nAvailable devices:")
    for i, (dev, desc) in enumerate(devices):
        print(f"[{i}] {dev} - {desc}")

    choice = input("\nSelect device number: ")
    try:
        idx = int(choice)
        return devices[idx][0]
    except (ValueError, IndexError):
        print("\nInvalid choice.")
        return None

def terminal(port, baudrate=115200):
    try:
        with serial.Serial(port, baudrate, timeout=1) as ser:
            print(f"\nConnected to {port} @ {baudrate} baud.")
            print("Type messages and press Enter to send.")
            while True:
                if ser.in_waiting:
                    data = ser.readline().decode(errors="ignore").strip()
                    if data:
                        print(f"< {data}")

                if ser.in_waiting == 0:
                    try:
                        msg = input("> ")
                        if msg.strip():
                            ser.write((msg + "\n").encode())
                    except EOFError:
                        break
    except serial.SerialException as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    while True:
        print("\n--- USB comms test (Embedded systems) ---")
        print("1. Choose device")
        print("2. Quit")
        option = input("\nSelect option: ")
        if option == "1":
            port = choose_device()
            if port:
                terminal(port)
        elif option == "2":
            break
        else:
            print("Invalid option.")
