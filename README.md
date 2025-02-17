# subCero
Your subnetting h**ero** written in C

## Features
- Calculates infos based on IP and CIDR:
    - Automatic Network Class calculation
    - IP Address
    - Netmask
    - Network- and broadcast address
    - Usable hosts range
    - Number of hosts
    - Number of subnets
- Features several checks to improve accuracy and reliability
- User-friendly input prompts and error handling
- Detailed output with all relevant subnetting information
- Statistics and permanent storage:
    - Tracks calculations and errors for the current session and all time
    - Displays the most common network class from saved data

## Planned Improvements
- Enhancements to user-friendliness
- Minor improvements in error handling

### Status
>[!IMPORTANT]
>- The program is considered complete for now. No major features are planned, only minor improvements and bug fixes.

## Not planned
- Graphical User Interface (Due to the current form of the program, a GUI would require a complete rewrite and is not considered practical)
- IPv6 support (Due to the complexity and the different approach required for IPv6 subnetting, this program focuses on IPv4 to keep it simple and educational)

## Usage
To use this program, follow these steps:

1. **Clone the repository**:
    ```sh
    git clone https://github.com/StefanBerecz/subCero.git
    cd subCero
    ```

2. **Compile the program**:
    ```sh
    gcc main.c calculations.c inOut.c -o main.out
    ```

3. **Run the program**:
    ```sh
    ./main.out
    ```

4. **Follow the prompts**: 
    - Enter the IP address and CIDR notation when prompted.
    - Review the detailed subnetting information provided by the program.

## Notice
>[!NOTE] 
>This project is intended for learning purposes. While it aims to help users understand the concepts of subnetting and network classes, it is primarily designed to enhance my coding skills. Contributions and suggestions are welcome!
