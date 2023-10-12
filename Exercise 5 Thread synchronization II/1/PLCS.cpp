START
CREATE thread for entry_guard_function  //creates a new thread for the entry_guard_function, which will continuously monitor for cars trying to enter the parking lot and either grant 
                                        //or deny them permission based on whether there is space available.
CREATE thread for exit_guard_function   //creates a new thread for the exit_guard_function, which will continuously monitor for cars trying to exit 
                                        //the parking lot and grant them permission while also updating the number of cars currently inside the parking lot.
CREATE thread for car_function          //creates a new thread for the car_function, which represents a car entering and exiting the parking lot.
                                        // Multiple instances of this function will be created and executed simultaneously by different threads.
WHILE program_running DO               //This loop runs continuously while the program_running flag is set to true.
    WAIT for random amount of time     //This line causes the program to pause for a random amount of time before continuing.
    CREATE new car_thread with car_function  //creates a new thread to represent a car entering and exiting the parking lot. The thread executes the car_function.
    WAIT for car to request permission to enter the parking lot  //causes the program to pause until the car thread requests permission to enter the parking lot.
    IF parking_lot_open AND num_cars_inside < MAX_CARS_INSIDE THEN  //This block of code checks whether there is space available in the parking lot 
                                                                    //(num_cars_inside is less than MAX_CARS_INSIDE) and whether the parking lot is open (parking_lot_open). If both conditions are true, 
                                                                    //the car is granted permission to enter the parking lot, and the num_cars_inside variable is incremented. Otherwise, permission is denied.
        GRANT permission to enter the parking lot 
        INCREMENT num_cars_inside
    ELSE
        DENY permission to enter the parking lot
    END IF
     WAIT for car to request permission to exit the parking lot   //wait for the car thread to request permission to exit the parking lot, then grant permission and decrement the num_cars_inside variable. 
                                                                //If there are no more cars in the parking lot (num_cars_inside is 0), the parking_lot_open flag is set to true.
    GRANT permission to exit the parking lot
    DECREMENT num_cars_inside
    IF num_cars_inside = 0 THEN
        SET parking_lot_open TO true
    END IF
END WHILE
END
 //represents the behavior of a car entering and exiting the parking lot. The car thread waits for a random amount of time before requesting permission to enter the parking lot. 
 //Once permission is granted, the car enters the parking lot and waits for a random amount of time before requesting permission to exit. Once permission is granted, the car exits the parking lot.
car_function:                 
WHILE true DO
    WAIT for random amount of time
    REQUEST permission to enter the parking lot
    WAIT for permission to enter the parking lot
    ENTER the parking lot
    WAIT for random amount of time
    REQUEST permission to exit the parking lot
    WAIT for permission to exit the parking lot
    EXIT the parking lot
END WHILE
//This function runs in its own thread and is responsible for managing entry into the parking lot. 
//It waits for a car to arrive, and if the parking lot is currently open and there is space available, 
//it grants permission to enter and increments the number of cars inside. If the parking lot is not open or there is no space, 
//it denies permission to enter. The function runs in an infinite loop to continuously check for cars arriving.
entry_guard_function:
WHILE true DO
    WAIT for car to arrive
    IF parking_lot_open AND num_cars_inside < MAX_CARS_INSIDE THEN
        GRANT permission to enter the parking lot
        INCREMENT num_cars_inside
    ELSE
        DENY permission to enter the parking lot
    END IF
END WHILE
//    This function runs in its own thread and is responsible for managing exit from the parking lot. It waits for a car to request permission to exit, 
//grants permission to exit, decrements the number of cars inside, and checks if the number of cars inside is now zero. If it is, it sets the parking_lot_open flag to true. T
//he function runs in an infinite loop to continuously check for cars requesting permission to exit.
exit_guard_function:
WHILE true DO
    WAIT for car to request permission to exit the parking lot
    GRANT permission to exit the parking lot
    DECREMENT num_cars_inside
    IF num_cars_inside = 0 THEN
        SET parking_lot_open TO true
    END IF
END WHILE
