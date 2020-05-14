int thresholds[8]={894,911,929,822,837,852,761,719};
char keypad_b[8]={'1','2','3','4','5','6','7','0'};

byte target_floor,current_floor,weight,turn_right_counter,turn_left_counter,stored_switch,floors=0;
bool target_floors[8]={false,false,false,false,false,false,false,false};
unsigned long prev,prev_switches,current,prev_leds=0;
bool stopped_flag,stored_flag=false;


void setup() {
DDRD = 0B11110000;
DDRB = 0B00000011;
attachInterrupt(digitalPinToInterrupt(2), GET_WEIGHT,CHANGE); //use interupt
}

void loop() {
current=millis();
if((unsigned long)current-prev_switches>200)
  checkKeys();
READ_KEYBAD();
if((unsigned long)current-prev_leds>200)
  Display_floor(current_floor);
Move_OR_Not();
}


void Elevator_Movement(){
UpdateTargetFloor();
if(current_floor<target_floor&&turn_right_counter<4)
  {
  prev=current;
  turn_right_counter++; 
    Turn_Clock_Wise();
  if(turn_right_counter==4)
  {
   current_floor++;
   turn_right_counter=0;
  }
  }
else if(current_floor>target_floor&&turn_left_counter<4)
{
  prev=current;
  turn_left_counter++; 
  Turn_anti_Clock_Wise();
if(turn_left_counter==4)
  {
   current_floor--;
   turn_left_counter=0;
  }
}
else if(current_floor==target_floor&&!stopped_flag)
  {
  stopped_flag=true;
  target_floors[current_floor]=false; 
  if(floors !=0)
	floors--;	 
  }
else;

}

void UpdateTargetFloor(){
byte targetup,targetdown=0;
for(int i=0;i<8;i++)
  if(target_floors[i]==true)
  {
   targetup=i;
   break;
  }
for(int i=7;i>=0;i--)
 if(target_floors[i]==true)
  {
  targetdown=i;
  break;
  }
if(abs(current_floor-targetup)<abs(current_floor-targetdown))
target_floor=targetup;
else if(abs(current_floor-targetup)>abs(current_floor-targetdown))
target_floor=targetdown;
else if(targetup==targetdown&&floors!=0)
target_floor=targetup;
}
void Move_OR_Not(){
GET_WEIGHT();
if(weight<70)
{
  if ((unsigned long)current-prev>150&&!stopped_flag)
  Elevator_Movement();
  if(stopped_flag&&(unsigned long)current-prev<1500);
    else stopped_flag=false;
PORTD &=0B11101111;
}
else PORTD |=0B00010000; 
}

void GET_TARGET_FLOOR(){
for(int i=0;i<8;i++)
{
if (target_floors[i]==true)
{
target_floor=i;
stopped_flag=false;
break;
}
}
}
void READ_KEYBAD(){
int Val=analogRead(A4);
for (int i=0;i<8;i++)
{
  if(Val-thresholds[i]==0)
  {
   int floor=(int)keypad_b[i];
   target_floors[floor-48]=true;
   GET_TARGET_FLOOR();
	floors++;  
  }
}
}

void GET_WEIGHT(){
weight = map(analogRead(A5),0,1023,0,100);}

void Turn_anti_Clock_Wise(){
switch(turn_left_counter){
case 1:
PORTB = 0B00000011;
break;
case 2:
PORTB = 0B00000001;
break;
case 3:
PORTB = 0B00000000;
break;
case 4: 
PORTB = 0B00000010;
break;
}
}

void Turn_Clock_Wise(){
switch(turn_right_counter){
case 1:
PORTB = 0B00000000;
break;
case 2:
PORTB = 0B00000001;
break;
case 3:
PORTB = 0B00000011;
break;
case 4: 
PORTB = 0B00000010;
break;
}
}

void Display_floor(int number){
prev_leds=current;
PORTD &=0B00011111;
switch (number)
{
case 0:
PORTD |=0B00001111;
break;
case 1: 
PORTD |=0B00100000;
break;
case 2:
PORTD |=0B01000000;
break; 
case 3:
PORTD |=0B01100000;
break;
case 4: 
PORTD  |=0B10000000;
break;
case 5:
PORTD |=0B10100000;
break;
case 6:
PORTD |=0B11000000;
break;
case 7: 
PORTD |=0B11111111;
break;
default:
PORTD |=0B00011111;
}
}
void checkKeys(){
prev_switches=current;
int switch_floor=0;
for (int i=10;i<=17;i++)
{ 
 if(digitalRead(i)==HIGH)
   {
	switch_floor=i;  
  if(!stored_flag)
  {
  stored_switch=switch_floor;
  stored_flag=true;
  }
  else{
  if(stored_switch==switch_floor)
    {
    switch(i){
    case 10:
    target_floors[0]=true;
    break;
    case 11:
    target_floors[1]=true;
    break;
    case 12:
    target_floors[2]=true;
    break;
    case 13:
    target_floors[3]=true;
    break;
    case 14:
    target_floors[4]=true;
    break;
    case 15:
    target_floors[5]=true;
    break;
    case 16:
    target_floors[6]=true;
    break;
    case 17:
    target_floors[7]=true;
    break;
    }
	floors++;
    stored_flag=false;
    }
    else{
    stored_flag=false;
    }
  } 
}
}
}
