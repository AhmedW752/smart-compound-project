#include "PRP_CASES.h"

void print_state (char * message ,char state){
     message[0]='0';
     message[1]= 'p';
     message[2]=  'c';
     message[3]=  state;
switch (state)
  {
    //------------------------------------------------------//
    /* owner case */
    //------------------------------------------------------//
    case owner_entered :
      //return "owner entered PRP";/*send string on request */
      
      break;
//    case owner_exited  :
//      return "owner_exited ";  
//      break;
    //-------------------------------------------------------------------//
    /*          guest cases       */
    //------------------------------------------------------------------//
    case visitor_entred :
     //return "Visitor_entered";
      break;
    case visitor_exited  :
      //return "guest_exited "; 
      break;
    //-----------------------------------------------------------------------//
    /*        service cases       */
     case service_exited :
    //return "service_exited";
      break;
    case service_entred  :
     //return "service_entered ";
      break;
   //----------------------------------------------------------------------------//
    /*  default case   */ 
     case card_taken :
     break;
     case card_written:      
     break;
     default:
     message[0]=0;
     message[1]= 0;
     message[2]=  0;
     message[3]=  0;
      break;
  }
}
/*
void space_lines()
{
  Serial.println("");
  Serial.println("//----------------------------------------------------------------------------//");
  Serial.println("");
  Serial.println("//----------------------------------------------------------------------------//");
  Serial.println("");
}*/
