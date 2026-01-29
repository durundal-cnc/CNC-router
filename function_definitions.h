// functions definition file
void startup_configuration(void); // this sets the startup configuration for the KFLOP/Kanalog/Konnect except for motor parameters
void init_x_motors(void); // set all the motor parameters
void init_y_motor(void);
void init_z_motor(void);
float analog_voltage_to_temperature(int); // convert the AI voltage to temperature
void check_temperatures(int);	// runs the temperature polling of all inputs
void check_limit_switches(void); // reads out all the limit switches
void read_external_buttons(void); // reads out all external buttons
float read_flow_meter(void); // reads the spindle flow meter flow rate
void safe_system(void); // safes the system (crash stops not normal stops)
int pause_motion(void); // stops at a convenient point for routine tool changes, stock changes, etc, returns 1 when done
int graceful_shutdown(void); // stops motion gracefully and readies system for power off, returns 1 when done
void ServiceMPG(void);
void service_spindle(void);
void ServiceToolsetter(void);
void ToolTableSet_am(void);
float service_time_insensitive(float);
int abs(int v);
float get_spindle_rpm_from_adc(void);
void update_velocity_DROs(void);
