# ESP-Now Example
This example shows how to get started with ESP-Now. It uses 2 ESP devices. 
The devices are classified into 2 types:

      • Master
      • Slave

In this example, the following steps are performed

      • The slave device(s) broadcast an access-point (AP)
      • The master device scans for the AP and adds the slave device(s) into its peer list
      • Callbacks are set, on each device, to send and receive data
      • The master reads data from digital IR sensor and sends that data to slave(s)
      • The slave device receives this data to turn on the LED and Buzzer connected to it.

# How to use the example

      • Just download the repository or copy the code from these files and upload it to their respective devices.
      • To incoporate more than 1 slave device, upload the slave code to other ESPs
      • Make suitable changes to the master program so that each device in its peer list is sent data
      • Refer our blog for more detailed explaination: https://www.robocraze.com/index.php/blog/tutorial-getting-started-with-esp-now.html
