#ifndef PRP_CASES_H
#define PRP_CASES_H
//---states for the RFID-----//
/*
 * 
 *#define default_case    'x' //"def"
/        owner cases       
#define owner_entered   '0' 
#define owner_exited    '1' 
#define owner_incprp    '2' 
/*        guest cases       
#define guest_entered    '4' 
#define guest_incprp    '5' 
#define guest_exited      '6' 
/*        guard cases       /
#define guard_ext       '8' 
#define guard_ent       '9' 
/*        ERASING CARDS    /
#define code_erased     '7' */
//---------------------------/
#define default_case    'x' //"def"
/*        owner cases       */
#define owner_entered   '0'
#define owner_exited    '1'
/*        service case      */
#define service_entred '2'
#define service_exited '3'
/*        guest cases       */
#define visitor_entred    '4'
#define visitor_exited      '5'
/*        card cases         */
#define card_taken        '6'
#define card_written      '7'
/*
          prototypes
*/

void print_state (char * message ,char state);
//void space_lines();
#endif
