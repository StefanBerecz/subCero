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

## Planned
- IPv6 support
- Statistics and permanent storage
- Graphical User Interface (eventually) 

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