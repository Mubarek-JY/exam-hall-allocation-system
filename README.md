# Exam Hall Seat Allocation System

**Section C - Group 6**

## Team Members
1. Meba Abebe-ETS0956/17
2. Moti Umeta - ETS 1054/17
3. Mesoud Foto - ETS 0992/17
4. Melaku Yospeh - ETS 0973/17
5. Mubarek Jemal - ETS 1058/17

## Project Description
A C++ console application for automated exam seating arrangement management with teacher and student login systems.

## Features

* Secure teacher PIN authentication
* Student registration, update, delete, and search
* Department-wise (sequential) and random seat allocation
* Configurable number of halls and seats per hall
* Student login to view seat assignment
* Login attempt tracking for teacher PIN (stored in memory)

## How to Compile and Run

g++ main.cpp -o seat_allocator
./seat_allocator


## Default Credentials

* Teacher PIN: `654321`
* Student ID format: `etsXXXXXX` (6 digits, case-insensitive)

## Usage Instructions

1. Teacher login using PIN
2. Register students
3. Configure halls and seats
4. Allocate seats using either:

   * Department-wise allocation (sequential allocation)
   * Random allocation (no seat conflicts)
5. Students can login to view their seat assignments

## Input Rules

* Student ID must be **9 characters**: `ets` + 6 digits
* Student ID is **case-insensitive** (`ETS` or `ets` accepted)
* Department must be one of: `SE`, `EE`, `CV`

## Project Structure

* `main.cpp` - Main program file
* `README.md` - Project documentation

## License

Academic Project - Section C Group 6
