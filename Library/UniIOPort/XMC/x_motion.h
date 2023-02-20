/********************************************************************************
;
;   X MOTION HEADER
;
;
;
;
;
;*******************************************************************************
; Rev 1.00 2012.01.01	Initiative	by jmsong
; Rev 1.01 2012.09.24	errmap table
; Rev 1.02 2012.11.02	abs, compare
; Rev 1.03 2013.02.15	pclr out
; Rev 1.04 2013.03.04	input counter, home 13
; Rev 1.05 2013.05.09	v_move(acc,dec)
;*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WINDOWS
#include <windows.h>

#define X_MOTION_API __stdcall
#else
#define X_MOTION_API

#endif

#ifndef _WINDEF_
//WinDef.h
typedef unsigned char       BYTE;	// 1byte
typedef unsigned short      WORD;	// 2byte
typedef unsigned long       DWORD;	// 4byte
#endif

#ifndef _BASETSD_H_  // Visual C++ 2008 
//basetsd.h
typedef signed char         INT8;
typedef signed short        INT16;
typedef signed int          INT32;
#endif

#if _MSC_VER < 1300	 // VC++ 6.0 MSC_VER:1200
#define		INT8	char
#define		INT16	short
#define		INT32	int
#endif

// Define Axis Source
#define		ID_NONE					0x00000000
#define		ID_HOME_SWITCH			0x00000001
#define		ID_POS_LIMIT			0x00000002
#define		ID_NEG_LIMIT   			0x00000004
#define		ID_AMP_FAULT			0x00000008

#define		ID_X_NEG_LIMIT 			0x00000040
#define		ID_X_POS_LIMIT 			0x00000080
#define		ID_ENC_Z				0x00000100
#define		ID_LATCH				0x00000200
#define		ID_AMP_POWER_OFF		0x00000800
#define		ID_INPOSITION			0x00002000


#define		NO_EVENT				0
#define		STOP_EVENT				1


// =============================== Motion Init=================================================
INT16 X_MOTION_API mmc_initx(void);										// motion board & library init

INT16 X_MOTION_API set_mmc_parameter_init(INT16 ax);					// axis parameter reset

INT16 X_MOTION_API get_version(void);									// dll sw version
INT16 X_MOTION_API get_bd_num(void);									// tatal motion board num
INT16 X_MOTION_API get_axis_num(void);									// tatal axis num
INT16 X_MOTION_API mmc_axes(INT16 bd, INT16 *ax);						// each axis num on board
INT16 X_MOTION_API version_chk(INT16 bd, INT16 *ver);					// motion hw version
INT32 X_MOTION_API get_board_id(INT16 bd, INT16 *idnum);

INT16 X_MOTION_API set_parameter(INT16 ax);						// parameter file save
INT16 X_MOTION_API get_parameter(INT16 ax);						// parameter file read

// =============================== Ex IO Init=================================================
INT16 X_MOTION_API mmc_exio_initx(void);
INT16 X_MOTION_API get_exio_bd_num(void);								// tatal ex io board num
INT16 X_MOTION_API get_exio_num(INT16 *exinport, INT16 *exoutport);		// tatal ex ioport num
INT16 X_MOTION_API mmc_exio(INT16 bd, INT16 *exinport, INT16 *exoutport);// each ex ioport num on board
INT16 X_MOTION_API exio_version_chk(INT16 bd, INT16 *ver);				// ex io hw version
INT32 X_MOTION_API get_exboard_id(INT16 bd, INT16 *idnum);

// =============================== Motion User IO=================================================
INT16 X_MOTION_API set_io(INT16 port, INT32 value);						// 0: On, 1: Off
INT16 X_MOTION_API get_io(INT16 port, INT32* value);
INT16 X_MOTION_API get_out_io(INT16 port, INT32* value);
INT16 X_MOTION_API set_bit(INT16 bitno);								// 0: On
INT16 X_MOTION_API reset_bit(INT16 bitno);

// =============================== Ex User IO=================================================
INT16 X_MOTION_API set_option_io(INT16 port, INT32 value);				// 0: On, 1: Off
INT16 X_MOTION_API get_option_io(INT16 port, INT32* value);
INT16 X_MOTION_API get_option_out_io(INT16 port, INT32* value);
INT16 X_MOTION_API set_option_bit(INT16 bitno);							// 0: On
INT16 X_MOTION_API reset_option_bit(INT16 bitno);

// =============================== Motion IO Setup=================================================
INT16 X_MOTION_API set_pulse_mode(INT16 ax, INT16 mode);
INT16 X_MOTION_API get_pulse_mode(INT16 ax, INT16 *mode);
INT16 X_MOTION_API set_encoder_mode(INT16 ax, INT16 mode);
INT16 X_MOTION_API get_encoder_mode(INT16 ax, INT16 *mode);
INT16 X_MOTION_API set_encoder_direction(INT16 ax, INT16 dir);
INT16 X_MOTION_API get_encoder_direction(INT16 ax, INT16 *dir);
INT16 X_MOTION_API set_amp_enable(INT16 ax, INT16 onoff);
INT16 X_MOTION_API get_amp_enable(INT16 ax, INT16 *onoff);
INT16 X_MOTION_API set_amp_reset(INT16 ax, INT16 onoff);
INT16 X_MOTION_API get_amp_reset(INT16 ax, INT16 *onoff);
INT16 X_MOTION_API set_amp_fault_level(INT16 ax, INT16 level);
INT16 X_MOTION_API get_amp_fault_level(INT16 ax, INT16 *level);
INT16 X_MOTION_API set_inposition_level(INT16 ax, INT16 level);
INT16 X_MOTION_API get_inposition_level(INT16 ax, INT16 *level);
INT16 X_MOTION_API set_home_level(INT16 ax, INT16 level);
INT16 X_MOTION_API get_home_level(INT16 ax, INT16 *level);
INT16 X_MOTION_API set_limit_level(INT16 ax, INT16 level);
INT16 X_MOTION_API get_limit_level(INT16 ax, INT16 *level);
INT16 X_MOTION_API set_latch_level(INT16 ax, INT16 level);
INT16 X_MOTION_API get_latch_level(INT16 ax, INT16 *level);

// =============================== Motion Status =============================================
INT16 X_MOTION_API in_sequence(INT16 ax);							// loading or execution of command
INT16 X_MOTION_API in_motion(INT16 ax);								// velocity is non zero
INT16 X_MOTION_API in_position(INT16 ax);							// inposition signal
INT16 X_MOTION_API motion_done(INT16 ax);							// !in_sequence !in_motion
INT16 X_MOTION_API axis_done(INT16 ax);								// motion_done in_position

INT32 X_MOTION_API axis_source(INT16 ax);
INT16 X_MOTION_API get_sts(INT16 ax, INT16 *msts, INT16 *ssts);
INT16 X_MOTION_API get_errint(INT16 ax, INT32 *errint);
INT16 X_MOTION_API reset_errint(INT16 ax);
INT16 X_MOTION_API get_eventint(INT16 ax, INT32 *eventint);
INT16 X_MOTION_API reset_eventint(INT16 ax);

INT16 X_MOTION_API wait_for_done(INT16 ax);
INT16 X_MOTION_API wait_for_all(INT16 n, INT16* axes);
INT16 X_MOTION_API axis_state(INT16 ax);
INT16 X_MOTION_API clear_status(INT16 ax);

INT16 X_MOTION_API home_switch(INT16 ax);
INT16 X_MOTION_API pos_switch(INT16 ax);
INT16 X_MOTION_API neg_switch(INT16 ax);
INT16 X_MOTION_API amp_fault_switch(INT16 ax);

// =============================== Position Counter =============================================
INT16 X_MOTION_API get_command(INT16 ax, double *command);
INT16 X_MOTION_API set_command(INT16 ax, double command);
INT16 X_MOTION_API get_position(INT16 ax, double *position);
INT16 X_MOTION_API set_position(INT16 ax, double position);
INT16 X_MOTION_API get_error(INT16 ax, double *counter);
INT16 X_MOTION_API set_error(INT16 ax, double counter);
INT16 X_MOTION_API get_mpg_counter(INT16 ax, double *counter);
INT16 X_MOTION_API set_mpg_counter(INT16 ax, double counter);
INT32 X_MOTION_API get_com_velocity(INT16 ax); // pps 32bit

// =============================== Alarm Stop Action  ==================================================
INT16 X_MOTION_API set_positive_sw_limit(INT16 ax, double pos, INT16 action);
INT16 X_MOTION_API get_positive_sw_limit(INT16 ax, double *pos, INT16 *action);
INT16 X_MOTION_API set_negative_sw_limit(INT16 ax, double pos, INT16 action);
INT16 X_MOTION_API get_negative_sw_limit(INT16 ax, double *pos, INT16 *action);
INT16 X_MOTION_API set_limit_action(INT16 ax, INT16 action);
INT16 X_MOTION_API get_limit_action(INT16 ax, INT16 *action);
INT16 X_MOTION_API set_amp_fault(INT16 ax, INT16 action);
INT16 X_MOTION_API get_amp_fault(INT16 ax, INT16 *action);

// =============================== Single Axis Motion =================================================

INT16 X_MOTION_API tv_move(INT16 ax, double strvel, double maxvel, INT16 acc);					// contiuous velocity move with trapezoidal acc
INT16 X_MOTION_API sv_move(INT16 ax, double strvel, double maxvel, INT16 acc);					// contiuous velocity move with s-curve acc
INT16 X_MOTION_API emg_stop(INT16 ax);
INT16 X_MOTION_API dec_stop(INT16 ax);

INT16 X_MOTION_API start_tr_imove(INT16 ax, double position, double strvel, double maxvel, INT16 acc, INT16 dec);		// trapezoidal relative move
INT16 X_MOTION_API start_ta_imove(INT16 ax, double position, double strvel, double maxvel, INT16 acc, INT16 dec);		// trapezoidal absolute move
INT16 X_MOTION_API start_sr_imove(INT16 ax, double position, double strvel, double maxvel, INT16 acc, INT16 dec);		// s curve relative move
INT16 X_MOTION_API start_sa_imove(INT16 ax, double position, double strvel, double maxvel, INT16 acc, INT16 dec);		// s curve absolute move

INT16 X_MOTION_API start_move(INT16 ax, double pos, double vel, INT16 acc);
INT16 X_MOTION_API move(INT16 ax, double pos, double vel, INT16 acc);
INT16 X_MOTION_API start_r_move(INT16 ax, double pos, double vel, INT16 acc);
INT16 X_MOTION_API r_move(INT16 ax, double pos, double vel, INT16 acc);
INT16 X_MOTION_API start_s_move(INT16 ax, double pos, double vel, INT16 acc);
INT16 X_MOTION_API s_move(INT16 ax, double pos, double vel, INT16 acc);
INT16 X_MOTION_API start_rs_move(INT16 ax, double pos, double vel, INT16 acc);
INT16 X_MOTION_API rs_move(INT16 ax, double pos, double vel, INT16 acc);

INT16 X_MOTION_API start_t_move(INT16 ax, double pos, double vel, INT16 acc, INT16 dec);
INT16 X_MOTION_API t_move(INT16 ax, double pos, double vel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_tr_move(INT16 ax, double pos, double vel, INT16 acc, INT16 dec);
INT16 X_MOTION_API tr_move(INT16 ax, double pos, double vel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_ts_move(INT16 ax, double pos, double vel, INT16 acc, INT16 dec);
INT16 X_MOTION_API ts_move(INT16 ax, double pos, double vel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_trs_move(INT16 ax, double pos, double vel, INT16 acc, INT16 dec);
INT16 X_MOTION_API trs_move(INT16 ax, double pos, double vel, INT16 acc, INT16 dec);

INT16 X_MOTION_API start_move_all(INT16 n, INT16* axes, double *pos, double *vel, INT16 *acc);
INT16 X_MOTION_API move_all(INT16 n, INT16* axes, double *pos, double *vel, INT16 *acc);
INT16 X_MOTION_API start_s_move_all(INT16 n, INT16* axes, double *pos, double *vel, INT16 *acc);
INT16 X_MOTION_API s_move_all(INT16 n, INT16* axes, double *pos, double *vel, INT16 *acc);
INT16 X_MOTION_API start_t_move_all(INT16 n, INT16* axes, double *pos, double *vel, INT16 *acc, INT16 *dec);
INT16 X_MOTION_API t_move_all(INT16 n, INT16* axes, double *pos, double *vel, INT16 *acc, INT16 *dec);
INT16 X_MOTION_API start_ts_move_all(INT16 n, INT16* axes, double *pos, double *vel, INT16 *acc, INT16 *dec);
INT16 X_MOTION_API ts_move_all(INT16 n, INT16* axes, double *pos, double *vel, INT16 *acc, INT16 *dec);

// =============================== Multi Axis Motion ==================================================
INT16 X_MOTION_API start_tr_imove_n(INT16 n, INT16* axes, double *pos, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_ta_imove_n(INT16 n, INT16* axes, double *pos, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_sr_imove_n(INT16 n, INT16* axes, double *pos, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_sa_imove_n(INT16 n, INT16* axes, double *pos, double strvel, double maxvel, INT16 acc, INT16 dec);

INT16 X_MOTION_API move_2ax(INT16 ax1, INT16 ax2, double x, double y, double vel, INT16 acc);
INT16 X_MOTION_API move_3ax(INT16 ax1, INT16 ax2, INT16 ax3, double x, double y, double z, double vel, INT16 acc);
INT16 X_MOTION_API move_4ax(INT16 ax1, INT16 ax2, INT16 ax3, INT16 ax4, double x, double y, double z, double w, double vel, INT16 acc);
INT16 X_MOTION_API move_nax(INT16 len, INT16 *ax, double *pos, double vel, INT16 acc);
INT16 X_MOTION_API smove_2ax(INT16 ax1, INT16 ax2, double x, double y, double vel, INT16 acc);
INT16 X_MOTION_API smove_3ax(INT16 ax1, INT16 ax2, INT16 ax3, double x, double y, double z, double vel, INT16 acc);
INT16 X_MOTION_API smove_4ax(INT16 ax1, INT16 ax2, INT16 ax3, INT16 ax4, double x, double y, double z, double w, double vel, INT16 acc);
INT16 X_MOTION_API smove_nax(INT16 len, INT16 *ax, double *pos, double vel, INT16 acc);

// =============================== Arc Motion =========================================================
INT16 X_MOTION_API start_tr_arc_n(INT16 n, INT16* axes, double centerx, double centery, double *endpos, INT16 dir, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_ta_arc_n(INT16 n, INT16* axes, double centerx, double centery, double *endpos, INT16 dir, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_sr_arc_n(INT16 n, INT16* axes, double centerx, double centery, double *endpos, INT16 dir, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_sa_arc_n(INT16 n, INT16* axes, double centerx, double centery, double *endpos, INT16 dir, double strvel, double maxvel, INT16 acc, INT16 dec);

INT16 X_MOTION_API start_tr_arc_deg_n(INT16 n, INT16* axes, double centerx, double centery, double degree, double endposz, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_ta_arc_deg_n(INT16 n, INT16* axes, double centerx, double centery, double degree, double endposz, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_sr_arc_deg_n(INT16 n, INT16* axes, double centerx, double centery, double degree, double endposz, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_sa_arc_deg_n(INT16 n, INT16* axes, double centerx, double centery, double degree, double endposz, double strvel, double maxvel, INT16 acc, INT16 dec);

INT16 X_MOTION_API start_tr_arc_pass_n(INT16 n, INT16* axes, double *passpos, double *endpos, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_ta_arc_pass_n(INT16 n, INT16* axes, double *passpos, double *endpos, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_sr_arc_pass_n(INT16 n, INT16* axes, double *passpos, double *endpos, double strvel, double maxvel, INT16 acc, INT16 dec);
INT16 X_MOTION_API start_sa_arc_pass_n(INT16 n, INT16* axes, double *passpos, double *endpos, double strvel, double maxvel, INT16 acc, INT16 dec);

INT16 X_MOTION_API arc_2ax(INT16 ax1, INT16 ax2, double x_center, double y_center, double angle, double vel, INT16 acc);
INT16 X_MOTION_API arc_3ax(INT16 ax1, INT16 ax2, INT16 ax3, double x_center, double y_center, double angle, double pos, double vel, INT16 acc);

// =============================== Continuous Path Motion =============================================
INT16 X_MOTION_API set_continuous_enable(INT16 ax, INT16 en);
INT16 X_MOTION_API get_continuous_enable(INT16 ax, INT16 *en);
INT16 X_MOTION_API get_continuous_buffer(INT16 ax);

// =============================== Override Motion =================================================
INT16 X_MOTION_API position_override(INT16 ax, double position);
INT16 X_MOTION_API velocity_override(INT16 ax, double velocity);
INT16 X_MOTION_API set_velocity_range(INT16 ax, double maxvel, INT16 en);
INT16 X_MOTION_API get_velocity_range(INT16 ax, double *maxvel, INT16 *en);

// =============================== MPG Motion =================================================
INT16 X_MOTION_API mpg_v_move(INT16 ax, double vellimit);
INT16 X_MOTION_API mpg_p_move(INT16 ax, double position, double vellimit);
INT16 X_MOTION_API set_mpg_mode(INT16 ax, INT16 mode);
INT16 X_MOTION_API get_mpg_mode(INT16 ax, INT16 *mode);
INT16 X_MOTION_API set_mpg_dir(INT16 ax, INT16 dir);
INT16 X_MOTION_API get_mpg_dir(INT16 ax, INT16 *dir);
INT16 X_MOTION_API set_mpg_ratio(INT16 ax, INT32 ratio);
INT16 X_MOTION_API get_mpg_ratio(INT16 ax, INT32 *ratio);

// =============================== Interrupt Event Done============================================
#ifdef _WINDOWS
INT16 X_MOTION_API mmc_int_enable(INT16 ax, HANDLE hevent);
INT16 X_MOTION_API mmc_int_disable(INT16 ax);
INT16 X_MOTION_API mmc_get_int_status(INT16 ax, DWORD *seni, DWORD *serror, DWORD *sevent);
#endif


// =============================== Special Function ================================================
INT16 X_MOTION_API homing(INT16 ax, INT16 dir, double strvel, double maxvel, double homvel, INT16 acc, INT16 mode);
INT16 X_MOTION_API set_electric_gear(INT16 ax, double ratio);
INT16 X_MOTION_API get_electric_gear(INT16 ax, double *ratio);
INT16 X_MOTION_API get_latched_position(INT16 ax, double *position);
INT16 X_MOTION_API set_correction(INT16 ax, INT16 type, double errpos, double vel);
INT16 X_MOTION_API get_correction(INT16 ax, INT16 *type, double *errpos);

INT16 X_MOTION_API set_amp_resolution(INT16 ax, INT32 resolution);
INT16 X_MOTION_API get_amp_resolution(INT16 ax, INT32 *resolution);
INT16 X_MOTION_API get_command_rpm(INT16 ax, INT16 *rpm_val);

INT16 X_MOTION_API mmc_register_reset(INT16 ax);

INT16 X_MOTION_API set_sensor_filter(INT16 ax, INT16 enable);
INT16 X_MOTION_API get_sensor_filter(INT16 ax, INT16 *enable);
INT16 X_MOTION_API set_jog_filter(INT16 ax, INT16 enable);
INT16 X_MOTION_API get_jog_filter(INT16 ax, INT16 *enable);
INT16 X_MOTION_API set_encoder_filter(INT16 ax, INT16 enable);
INT16 X_MOTION_API get_encoder_filter(INT16 ax, INT16 *enable);
INT16 X_MOTION_API set_mpg_filter(INT16 ax, INT16 enable);
INT16 X_MOTION_API get_mpg_filter(INT16 ax, INT16 *enable);

#define		MAX_ErrMap_TABLE_N			(100)
INT16 X_MOTION_API set_errmap_enable(INT16 ax, INT16 en);
INT16 X_MOTION_API get_errmap_enable(INT16 ax, INT16 *en);
INT16 X_MOTION_API set_errmap_table(INT16 ax, INT16 table_n, double *position, double *error);
INT16 X_MOTION_API get_errmap_table(INT16 ax, INT16 *table_n, double *position, double *error);

INT16 X_MOTION_API set_latch_flag_enable(INT16 ax, INT16 en);
INT16 X_MOTION_API get_latch_flag_enable(INT16 ax, INT16 *en);

#define		MMC_ABS_RX_SIZE				(40)
INT16 X_MOTION_API get_abs_serial(INT16 ax, INT16 type, char *reg);
INT16 X_MOTION_API set_abs_position(INT16 ax, INT16 type, INT32 multiturn, INT32 singleturn);

INT16 X_MOTION_API set_position_compare1(INT16 ax, INT16 en, INT16 counter, INT16 mode, double position);
INT16 X_MOTION_API get_position_compare1(INT16 ax, INT16 *en, INT16 *counter, INT16 *mode, double *position);
INT16 X_MOTION_API set_position_compare2(INT16 ax, INT16 en, INT16 counter, INT16 mode, double position);
INT16 X_MOTION_API get_position_compare2(INT16 ax, INT16 *en, INT16 *counter, INT16 *mode, double *position);

INT16 X_MOTION_API set_pclr_out(INT16 ax, INT16 level);
INT16 X_MOTION_API get_pclr_out(INT16 ax, INT16 *level);

INT16 X_MOTION_API set_input_counter(INT16 bd, INT32 counter0, INT32 counter1);
INT16 X_MOTION_API get_input_counter(INT16 bd, INT32* counter0, INT32* counter1);

INT16 X_MOTION_API v_move(INT16 ax, double strvel, double maxvel, INT16 acc, INT16 dec, INT16 scurve); // V1.05

INT16 X_MOTION_API set_compare_port_config(INT16 port, INT16 bitn, INT16 ax); // V1.07
INT16 X_MOTION_API get_compare_port_config(INT16 port, INT16 bitn, INT16 *ax);
INT16 X_MOTION_API set_compare_level(INT16 ax, INT16 compare_no, INT16 level);
INT16 X_MOTION_API get_compare_level(INT16 ax, INT16 compare_no, INT16 *level);


// Error Return Define ===============================================
#define		MMC_OK						(0) // No Err
#define		MMC_NOT_FOUND_ERR			(1) // MMC Not Founded Err
#define		MMC_DRIVER_OPEN_ERR			(2) // Device Driver Open Err
#define		MMC_FILE_PARA_ERR			(3) // File Open Read Write Err
#define		MMC_ABS_TIME_OUT_ERR		(4) // ABS Serial Comm Time Out
#define		MMC_MUTEX_OPEN_ERR			(5) // Mutex Open Err
#define		MMC_MUTEX_TIME_ERR			(6) // Mutex Time Out

#define		MMC_INVALID_AXIS			(7) // Invalid Axis Number
#define		MMC_INVALID_PARAMETER		(8) // Invalid Parameter
#define		MMC_NOT_INITIALIZED			(9) // Not Initialized
#define		MMC_INVALID_SPEED			(10)// Invalid Speed
#define		MMC_INVALID_AXES			(11)// Invalid Axes for Multi Axis
#define		MMC_AMP_OFF					(12)// Amp Off Status
#define		MMC_INVALID_ACCDEC			(13)// Invalid Acc Dec
#define		MMC_NOT_READY				(14)// Continuous Move Not Ready
#define		MMC_INVALID_PORT			(15)// Invalid Port Number
#define		MMC_ABS_DATA_ERR			(16)// ABS Serial Data Err


/*
This Function support Event-Handler, and may reduce CPU-Usage in While-Loop.
void DoEvents(void)
{
	MSG       msg;
	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE ))
	{
		 TranslateMessage(&msg);
		 DispatchMessage(&msg);
	}
	Sleep(1);
}
*/

#ifdef __cplusplus
}
#endif
