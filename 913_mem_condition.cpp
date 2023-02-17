#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
using namespace std;
int const n = 60; 
static uint64_t s[4];
static uint64_t x = 0xbf3749f5b97cd3b9; /* The state can be seeded with any value. */
double r; // random number

uint64_t next64() {  // random number
    uint64_t z = (x += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}

void rnd256_init() { // random number
    s[0] = next64();  s[1] = next64();  s[2] = next64();  s[3] = next64();
}

int correct(int (*Error_vector)[n], int a, int B);
int SQerror(int (*Record_error)[n], int a, double probability); // Single qubit error
int TQerror(int (*Record_error)[n], int a, int b, double probability); // Two qubit error
int CNOT(int (*Error_vector)[n], int a, int b);
int flag1a(int (*Error_vector)[n], double probability, double Gamma, double beta);
int flag1b(int (*Error_vector)[n], double probability, double Gamma, double beta);
int flag_rise1(int (*Error_vector)[n], double probability, double Gamma, double beta);
int unflagged(int (*Error_vector2)[n], double probability, double Gamma, double beta);
int separate(int (*Error_vector)[n], double probability, double Gamma, double beta);
int perfect(int (*Error_vector2)[n] );

int main(){
	// counter 
    int input;
    clock_t start, end;
    start = clock();
    // print time
    time_t current_time;
    char* c_time_string;
    current_time = time(NULL);
    c_time_string = ctime(&current_time);
    printf("Current time is %s", c_time_string);
    
    double Total_number, Error_number, probability;
    int s1, s2, s3, s4, s5, s6, s7, s8; 
    int s9,s10,s11,s12,s13,s14,s15,s16;
    int m1, m2, m3, m4,m5,m6,m7,m8, q1, q2;
    int N = 10; // 切 N 個格子
    int Error_vector[2][n]={0};
    int xe[1]={2}   ; // sigle error by user , X, Z, Y = 2, 3, 4
    int ze[1]={3}   ; // sigle error by user , X, Z, Y = 2, 3, 4
    double Display_p[N], Display_e[N];
    
    double Gamma = 0; // gamma = error memory rate of idle qubits / gate error rate
	double beta = 1; 
    
    cout << "[[9,1,3]] parallel, memeory threshold" << " ,loop=" << N-1 << " ,gamma=" << Gamma << endl << endl << endl;
    
    for(int i=1; i<N; i++){
        //probability = 0.0001+0.0001*(i); // gamma=1 ; N=4
        probability = 0.0001+0.0001*(i-1)*(i*i/2); // gamma=1 ; N=4
	Total_number = 0; // 總共執行電路的次數
        Error_number = 0; // 測到是電路的 output 是 malignant pair的次數
      // while(Total_number<6000000||Error_number<30){ // Error_number  數量
        while(Total_number< 100000000){
            s1 = 0;
            s2 = 0;
            s3 = 0;
            s4 = 0;
            s5 = 0;
            s6 = 0;
            s7 = 0;
            s8 = 0;
            s9 = 0;
            s10= 0;
            s11= 0;
            s12= 0;
            s13= 0;
            s14= 0;
            s15= 0;
            s16= 0;
            m1 = 0;
            m2 = 0;
            m3 = 0;
            m4 = 0;
            m5 = 0;
            m6 = 0;
            q1 = 0;
            q2 = 0;
            
            //**get time***************************
            time_t current_time;
    		char* c_time_string;
   			current_time = time(NULL);
   			c_time_string = ctime(&current_time);
            //*************************************
            for(int i=0; i<2; i++){
                for(int j=0; j<n; j++)	Error_vector[i][j] = 0; // 在進入新的電路之前, 將error vector歸零 (第一列: 紀錄 X error, 第二列: 紀錄 Z error)
            }

     
            // flag (1-A) 
        	flag1a(Error_vector, probability, Gamma, beta );
   
    //====================================================================
  
            m1 = Error_vector[0][9];
            m2 = Error_vector[0][10];
            m3 = Error_vector[0][11];
            m4 = Error_vector[0][12];
            m5 = Error_vector[0][13];
            m6 = Error_vector[0][14];
	
					
			if ( m1==1 || m2==1 || m3==1 || m4==1 || m5==1 || m6==1 ){
				
				for(int i=0; i<2; i++){
                	for(int j=9; j<n; j++)	Error_vector[i][j] = 0; // 在進入新的電路之前, 將error vector歸零 (第一列: 紀錄 X error, 第二列: 紀錄 Z error)
            	}
            unflagged(Error_vector, probability, Gamma, beta );		
			}

  
   //====================================================================  

   
			if ( m1==0 && m2==0 && m3==0 && m4==0 && m5==0 && m6==0   ){ 
    	
   
               // flag (1-B) 
            flag1b(Error_vector, probability, Gamma, beta);
           
          
          
            // flag  trigger
            	if ( Error_vector[0][18] == 1  ){
            		
            	flag_rise1(Error_vector, probability, Gamma, beta);
           
                // special case
                // X23456
                if (  Error_vector[0][49]==1 && Error_vector[0][50]==0 && Error_vector[0][51]==0 && Error_vector[0][52]==0 && Error_vector[0][53]==0 && Error_vector[0][54]==0 && Error_vector[0][55]==0 && Error_vector[0][57]==0){
                    correct(Error_vector, 2, xe[0]);
                    correct(Error_vector, 3, xe[0]);
                    correct(Error_vector, 4, xe[0]);
                    correct(Error_vector, 5, xe[0]);
                    correct(Error_vector, 6, xe[0]);
                }
                // X2456
                if (  Error_vector[0][49]==1 && Error_vector[0][50]==1 && Error_vector[0][51]==0 && Error_vector[0][52]==0 && Error_vector[0][53]==0 && Error_vector[0][54]==0 && Error_vector[0][55]==0 && Error_vector[0][57]==0) {
                    correct(Error_vector, 2, xe[0]);
                    correct(Error_vector, 4, xe[0]);
                    correct(Error_vector, 5, xe[0]);
                    correct(Error_vector, 6, xe[0]);
                }
                // X246
                if (  Error_vector[0][49]==1 && Error_vector[0][50]==1 && Error_vector[0][51]==1 && Error_vector[0][52]==1 && Error_vector[0][53]==0 && Error_vector[0][54]==0 && Error_vector[0][55]==0 && Error_vector[0][57]==0) {
                    correct(Error_vector, 2, xe[0]);
                    correct(Error_vector, 4, xe[0]);
                    correct(Error_vector, 6, xe[0]);
                }
                // X26
                if (  Error_vector[0][49]==1 && Error_vector[0][50]==1 && Error_vector[0][51]==0 && Error_vector[0][52]==1 && Error_vector[0][53]==0 && Error_vector[0][54]==0 && Error_vector[0][55]==0 && Error_vector[0][57]==0) {
                    correct(Error_vector, 2, xe[0]);
                    correct(Error_vector, 4, xe[0]);
                    correct(Error_vector, 6, xe[0]);
                }
               
                 // special case
               
                // X45689
                if (  Error_vector[0][49]==0 && Error_vector[0][50]==0 && Error_vector[0][51]==0 && Error_vector[0][52]==0 && Error_vector[0][53]==1 && Error_vector[0][54]==0  ) {
                    correct(Error_vector, 4, xe[0]);
                    correct(Error_vector, 5, xe[0]);
                    correct(Error_vector, 6, xe[0]);
                    correct(Error_vector, 8, xe[0]);
                    correct(Error_vector, 9, xe[0]);;
                }
                // X5789
                if (  Error_vector[0][49]==0 && Error_vector[0][50]==0 && Error_vector[0][51]==0 && Error_vector[0][52]==0 && Error_vector[0][53]==1 && Error_vector[0][54]==1  ) {
                    correct(Error_vector, 4, xe[0]);
                    correct(Error_vector, 5, xe[0]);
                    correct(Error_vector, 6, xe[0]);
                    correct(Error_vector, 8, xe[0]);
                }                
                 // X579
                if (  Error_vector[0][49]==0 && Error_vector[0][50]==0 && Error_vector[0][51]==1 && Error_vector[0][52]==1 && Error_vector[0][53]==1 && Error_vector[0][54]==1  ) {
                    correct(Error_vector, 4, xe[0]);
                    correct(Error_vector, 6, xe[0]);
                    correct(Error_vector, 8, xe[0]);
                }               
                // X59
                if (  Error_vector[0][49]==0 && Error_vector[0][50]==0 && Error_vector[0][51]==1 && Error_vector[0][52]==1 && Error_vector[0][53]==0 && Error_vector[0][54]==0  ) {
                    correct(Error_vector, 4, xe[0]);
                    correct(Error_vector, 6, xe[0]);
                }   
                
           else if(Error_vector[1][15] == 1 || Error_vector[1][17] == 1 ){
            	// flag no triggher, but measurement trigger.

            	separate(Error_vector, probability, Gamma, beta );

				}  		
            }

}


//---Perfect circuit----------------------------------------------------------------------------------------------------
	
     		for(int i=0; i<2; i++){
                for(int j=9; j<n; j++)	Error_vector[i][j] = 0; // 在進入新的電路之前, 將error vector歸零 (第一列: 紀錄 X error, 第二列: 紀錄 Z error)
            }

			perfect(Error_vector );	
                

		// 排除stabilzer & degenerate error	
	        s1 = ( Error_vector[0][0] * 1  + Error_vector[0][1] * 1 + Error_vector[0][2] * 0 + Error_vector[0][3] * 0 + Error_vector[0][4] * 0 + Error_vector[0][5] * 0 + Error_vector[0][6] * 0 + Error_vector[0][7] * 0 + Error_vector[0][8] * 0 )%2   ;
            s2 = ( Error_vector[0][0] * 0  + Error_vector[0][1] * 1 + Error_vector[0][2] * 1 + Error_vector[0][3] * 0 + Error_vector[0][4] * 0 + Error_vector[0][5] * 0 + Error_vector[0][6] * 0 + Error_vector[0][7] * 0 + Error_vector[0][8] * 0 )%2   ;
            s3 = ( Error_vector[0][0] * 0  + Error_vector[0][1] * 0 + Error_vector[0][2] * 0 + Error_vector[0][3] * 1 + Error_vector[0][4] * 1 + Error_vector[0][5] * 0 + Error_vector[0][6] * 0 + Error_vector[0][7] * 0 + Error_vector[0][8] * 0 )%2   ;
            s4 = ( Error_vector[0][0] * 0  + Error_vector[0][1] * 0 + Error_vector[0][2] * 0 + Error_vector[0][3] * 0 + Error_vector[0][4] * 1 + Error_vector[0][5] * 1 + Error_vector[0][6] * 0 + Error_vector[0][7] * 0 + Error_vector[0][8] * 0 )%2   ;
            s5 = ( Error_vector[0][0] * 0  + Error_vector[0][1] * 0 + Error_vector[0][2] * 0 + Error_vector[0][3] * 0 + Error_vector[0][4] * 0 + Error_vector[0][5] * 0 + Error_vector[0][6] * 1 + Error_vector[0][7] * 1 + Error_vector[0][8] * 0 )%2   ; 
            s6 = ( Error_vector[0][0] * 0  + Error_vector[0][1] * 0 + Error_vector[0][2] * 0 + Error_vector[0][3] * 0 + Error_vector[0][4] * 0 + Error_vector[0][5] * 0 + Error_vector[0][6] * 0 + Error_vector[0][7] * 1 + Error_vector[0][8] * 1 )%2   ; 
            s7 = ( Error_vector[1][0] * 1  + Error_vector[1][1] * 1 + Error_vector[1][2] * 1 + Error_vector[1][3] * 1 + Error_vector[1][4] * 1 + Error_vector[1][5] * 1 + Error_vector[1][6] * 0 + Error_vector[1][7] * 0 + Error_vector[1][8] * 0 )%2   ;
            s8 = ( Error_vector[1][0] * 0  + Error_vector[1][1] * 0 + Error_vector[1][2] * 0 + Error_vector[1][3] * 1 + Error_vector[1][4] * 1 + Error_vector[1][5] * 1 + Error_vector[1][6] * 1 + Error_vector[1][7] * 1 + Error_vector[1][8] * 1 )%2   ;
        	
			s9 = ( Error_vector[1][0] * 1  + Error_vector[1][1] * 1 + Error_vector[1][2] * 0 + Error_vector[1][3] * 0 + Error_vector[1][4] * 0 + Error_vector[1][5] * 0 + Error_vector[1][6] * 0 + Error_vector[1][7] * 0 + Error_vector[1][8] * 0 )%2   ;
            s10 = ( Error_vector[1][0] * 0  + Error_vector[1][1] * 1 + Error_vector[1][2] * 1 + Error_vector[1][3] * 0 + Error_vector[1][4] * 0 + Error_vector[1][5] * 0 + Error_vector[1][6] * 0 + Error_vector[1][7] * 0 + Error_vector[1][8] * 0 )%2   ;
		    s11 = ( Error_vector[1][0] * 0  + Error_vector[1][1] * 0 + Error_vector[1][2] * 0 + Error_vector[1][3] * 1 + Error_vector[1][4] * 1 + Error_vector[1][5] * 0 + Error_vector[1][6] * 0 + Error_vector[1][7] * 0 + Error_vector[1][8] * 0 )%2   ;
            s12 = ( Error_vector[1][0] * 0  + Error_vector[1][1] * 0 + Error_vector[1][2] * 0 + Error_vector[1][3] * 0 + Error_vector[1][4] * 1 + Error_vector[1][5] * 1 + Error_vector[1][6] * 0 + Error_vector[1][7] * 0 + Error_vector[1][8] * 0 )%2   ;
            s13 = ( Error_vector[1][0] * 0  + Error_vector[1][1] * 0 + Error_vector[1][2] * 0 + Error_vector[1][3] * 0 + Error_vector[1][4] * 0 + Error_vector[1][5] * 0 + Error_vector[1][6] * 1 + Error_vector[1][7] * 1 + Error_vector[1][8] * 0 )%2   ; 
            s14 = ( Error_vector[1][0] * 0  + Error_vector[1][1] * 0 + Error_vector[1][2] * 0 + Error_vector[1][3] * 0 + Error_vector[1][4] * 0 + Error_vector[1][5] * 0 + Error_vector[1][6] * 0 + Error_vector[1][7] * 1 + Error_vector[1][8] * 1 )%2   ; 
            s15 = ( Error_vector[0][0] * 1  + Error_vector[0][1] * 1 + Error_vector[0][2] * 1 + Error_vector[0][3] * 1 + Error_vector[0][4] * 1 + Error_vector[0][5] * 1 + Error_vector[0][6] * 0 + Error_vector[0][7] * 0 + Error_vector[0][8] * 0 )%2   ;
            s16 = ( Error_vector[0][0] * 0  + Error_vector[0][1] * 0 + Error_vector[0][2] * 0 + Error_vector[0][3] * 1 + Error_vector[0][4] * 1 + Error_vector[0][5] * 1 + Error_vector[0][6] * 1 + Error_vector[0][7] * 1 + Error_vector[0][8] * 1 )%2   ;
        
			
			if ( s1==0 && s2==0 && s3==0 && s4==0 && s5==0 && s6==0 && s7==0 && s8==0 ){
				goto ckp3;
			}
			if ( s9==0 && s10==0 && s11==0 && s12==0 && s13==0 && s14==0 && s15==0 && s16==0 ){
				goto ckp3;
			}	
					
		//排除logci X, logic Z 
			q1  = ( Error_vector[0][0] * 1  + Error_vector[0][1] * 1 + Error_vector[0][2] * 1 + Error_vector[0][3] * 1 + Error_vector[0][4] * 1 + Error_vector[0][5] * 1 + Error_vector[0][6] * 1 + Error_vector[0][7] * 1 + Error_vector[0][8] * 1 )%2   ;
            q2 = ( Error_vector[1][0] * 1  + Error_vector[1][1] * 1 + Error_vector[1][2] * 1 + Error_vector[1][3] * 1 + Error_vector[1][4] * 1 + Error_vector[1][5] * 1 + Error_vector[1][6] * 1 + Error_vector[1][7] * 1 + Error_vector[1][8] * 1 )%2   ;
	      
	       if (q1 == 0  && q2 ==0){
	       		goto ckp3;
			};
	
			
		Error_number = Error_number + 1 ;	
			
	
        ckp3:
                
                
                Total_number = Total_number + 1;
                //cout << Error_number << ", " << Total_number << endl;

                
        } // while
        Display_p[i-1] = probability;
        Display_e[i-1] = Error_number/Total_number;
       	cout  <<  "No." << i << " Physical error rate : " << probability << "; Error number = " << Error_number << ", total_number = " << Total_number << ", logical error rate : " <<  Error_number/(Total_number) << "    |" << c_time_string ;//<< endl;
	
	
	
	
	} // i loop
   
    cout << "Physical_error_rate = [ ";
    for(int i=0; i<N-1; i++){
        if(i<N-2)	cout <<  Display_p[i] << ", ";
        if(i==N-2) cout << Display_p[i];
    } cout << " ]" << endl;

    cout << "Logical_error_rate = [ ";
    for(int i=0; i<N-1; i++){
        if(i<N-2)	cout <<  Display_e[i] << ", ";
        if(i==N-2) cout << Display_e[i];
    } cout << " ]" << endl;
    
    cout << "Parallel & " << "Gamma = " << Gamma << endl;
    
    
    
    end = clock();
    double diff = end - start; // ms
    printf(" %f  sec", diff / CLOCKS_PER_SEC );
      scanf("%d", &input);
    
    return 0;
}

int correct(int (*Error_vector)[n], int a, int B){
	a = a - 1;
	if( B==2 ){
		*(*(Error_vector+0)+a) = (*(*(Error_vector+0)+a) + 1)%2;   // X error時 ,再該qubit補上一個error count
	//	cout << "X error on " << a+1 << endl;
	}
	if( B==3 ){
		*(*(Error_vector+1)+a) = (*(*(Error_vector+1)+a) + 1)%2;    // Z error時,再該qubit補上一個error count
	//	cout << "Z error on " << a+1 << endl;
	}
	if( B==4 ){
		*(*(Error_vector+0)+a) = (*(*(Error_vector+0)+a) + 1)%2;    // Y error時,再該qubit補上一個X & Y error count
		*(*(Error_vector+1)+a) = (*(*(Error_vector+1)+a) + 1)%2;
	//	cout << "Y error on " << a+1 << endl;
	}
	
}

int SQerror(int (*Error_vector)[n], int a, double probability){
    a = a - 1;
    r = next64()/(pow(2,64));
    if( r < (probability/3) ){ // depolarizing channel, r < (1/3)*p 發生 X error
        *(*(Error_vector+0)+a) = (*(*(Error_vector+0)+a) + 1)%2;
    }
    if( r > (probability/3) && r < (2*probability/3) ){ // (1/3)*p < r < (2/3)*p 發生 Z error
        *(*(Error_vector+1)+a) = (*(*(Error_vector+1)+a) + 1)%2;
    }
    if( r > (2*probability/3) && r < (probability) ){ // (2/3)*p < r < p 發生 Y error
        *(*(Error_vector+0)+a) = (*(*(Error_vector+0)+a) + 1)%2;
        *(*(Error_vector+1)+a) = (*(*(Error_vector+1)+a) + 1)%2;
    }
    
}

int TQerror(int (*Error_vector)[n], int a, int b, double probability){
    SQerror(Error_vector, a, probability);
    SQerror(Error_vector, b, probability);
}

int CNOT(int (*Error_vector)[n], int a, int b){
    a = a - 1;
    b = b - 1;
    if(Error_vector[0][a]==1){                                     // X error 發生
        Error_vector[0][b] = (Error_vector[0][b] + 1)%2;
    }
    if(Error_vector[1][b]==1){    								   // Z error 發生
        Error_vector[1][a] = (Error_vector[1][a] + 1)%2;
    }
    
}



int unflagged(int (*Error_vector)[n], double probability, double Gamma, double beta){
				
     		for(int i=0; i<2; i++){
                for(int j=9; j<n; j++)	Error_vector[i][j] = 0; // 在進入新的電路之前, 將error vector歸零 (第一列: 紀錄 X error, 第二列: 紀錄 Z error)
            }

			CNOT(Error_vector, 1, 10);
            TQerror(Error_vector, 1, 10, probability*beta);
            CNOT(Error_vector, 2, 11);
            TQerror(Error_vector, 2, 11, probability*beta);
            CNOT(Error_vector, 4, 12);
            TQerror(Error_vector, 4, 12, probability*beta);
            CNOT(Error_vector, 5, 13);
            TQerror(Error_vector, 5, 13, probability*beta);
            CNOT(Error_vector, 7, 14);
            TQerror(Error_vector, 7, 14, probability*beta);           
            CNOT(Error_vector, 8, 15);
            TQerror(Error_vector, 8, 15, probability*beta);     
            SQerror(Error_vector, 3, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
            
            
            CNOT(Error_vector, 2, 10);
            TQerror(Error_vector, 2, 10, probability*beta);
            CNOT(Error_vector, 3, 11);
            TQerror(Error_vector, 3, 11, probability*beta);
            CNOT(Error_vector, 5, 12);
            TQerror(Error_vector, 5, 12, probability*beta);
            CNOT(Error_vector, 6, 13);
            TQerror(Error_vector, 6, 13, probability*beta);
            CNOT(Error_vector, 8, 14);
            TQerror(Error_vector, 8, 14, probability*beta);           
            CNOT(Error_vector, 9, 15);
            TQerror(Error_vector, 9, 15, probability*beta);     
            SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 7, probability*Gamma);  

	
	//flag 1b
    
            CNOT(Error_vector, 16, 1);
            TQerror(Error_vector, 16, 1, probability*beta);
                    //  SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);   
            
   			CNOT(Error_vector, 16, 3);
   			TQerror(Error_vector, 16, 3, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
        //    SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);   
            
     		CNOT(Error_vector, 16, 5);
     		TQerror(Error_vector, 16, 5, probability*beta);
     		SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
        //    SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
     		
    		CNOT(Error_vector, 16, 4);
    		TQerror(Error_vector, 16, 4, probability*beta);
    	   	SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
        //    SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);	
    		
    		
    		CNOT(Error_vector, 16, 2);
    		TQerror(Error_vector, 16, 2, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
        //    SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
    		
     		
     		CNOT(Error_vector, 16, 6);
     		TQerror(Error_vector, 16, 6, probability*beta);
    		SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
        //    SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
     		
     		
//flag 1c
			CNOT(Error_vector, 18, 4);
			TQerror(Error_vector, 18, 4, probability*beta);
 		
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
        //    SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
 		
 		
  			CNOT(Error_vector, 18, 6);
  			TQerror(Error_vector, 18, 6, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
        //    SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
  			
   			CNOT(Error_vector, 18, 8);
   			TQerror(Error_vector, 18, 8, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
        //    SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
   			
   			CNOT(Error_vector, 18, 7);
   			TQerror(Error_vector, 18, 7, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
        //    SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
            
  			CNOT(Error_vector, 18, 5);
  			TQerror(Error_vector, 18, 5, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
        //    SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);	
    	
    		CNOT(Error_vector, 18, 9);		
    		TQerror(Error_vector, 18, 9, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
        //    SQerror(Error_vector, 9, probability*Gamma);
	

	        SQerror(Error_vector, 16, probability*Gamma);   
            SQerror(Error_vector, 17, probability*Gamma);
            SQerror(Error_vector, 18, probability*Gamma);
            
	     	// ECC correct X error	
				// X1 error
                if (  Error_vector[0][9] == 1 && Error_vector[0][10]  == 0  ){
                    correct(Error_vector, 1, 2);
                    }
                // X2 error
                if (  Error_vector[0][9] == 1 && Error_vector[0][10]  == 1  ){
                    correct(Error_vector, 2, 2);
                    }
                // X3 error
                if (  Error_vector[0][9] == 0 && Error_vector[0][10]  == 1  ){
                    correct(Error_vector, 3, 2);
                    }
                // X4 error
                if (   Error_vector[0][11] == 1 && Error_vector[0][12]  == 0  ){
                    correct(Error_vector, 4, 2);
                    }
                // X5 error
                if (   Error_vector[0][11] == 1 && Error_vector[0][12]  == 1  ){
                    correct(Error_vector, 5, 2);
                    }
                // X6 error
                if (   Error_vector[0][11] == 0 && Error_vector[0][12]  == 1  ){
                    correct(Error_vector, 6, 2);
                    }
           
				// X7 error
				if (   Error_vector[0][13] == 1 && Error_vector[0][14]  == 0 ){
                    correct(Error_vector, 7, 2);
                    }
      
                // X8 error                
				if (   Error_vector[0][13] == 1 && Error_vector[0][14]  == 1 ){
                    correct(Error_vector, 8, 2);
                    }
                   
                // X9 error                
				if (   Error_vector[0][13] == 0 && Error_vector[0][14]  == 1 ){
                    correct(Error_vector, 9, 2);
                    }  		
					
       
                // ECC correct Z error	
		
				if (Error_vector[1][15] == 1 && Error_vector[1][17] == 0 ){
				    correct(Error_vector, 1, 3);	
				}
				if (Error_vector[1][15] == 0 && Error_vector[1][17] == 1 ){
				    correct(Error_vector, 7, 3);		
				}	
			    if (Error_vector[1][15] == 1 && Error_vector[1][17] == 1 ){
				    correct(Error_vector, 4, 3);		
				}        		
							

}


int separate(int (*Error_vector)[n], double probability, double Gamma, double beta){
				
     		for(int i=0; i<2; i++){
                for(int j=9; j<n; j++)	Error_vector[i][j] = 0; // 在進入新的電路之前, 將error vector歸零 (第一列: 紀錄 X error, 第二列: 紀錄 Z error)
            }


            
	//flag 1b
    
            CNOT(Error_vector, 16, 1);
            TQerror(Error_vector, 16, 1, probability*beta);
                    //  SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);   
            
   			CNOT(Error_vector, 16, 3);
   			TQerror(Error_vector, 16, 3, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
        //    SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);   
            
     		CNOT(Error_vector, 16, 5);
     		TQerror(Error_vector, 16, 5, probability*beta);
     		SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
        //    SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
     		
    		CNOT(Error_vector, 16, 4);
    		TQerror(Error_vector, 16, 4, probability*beta);
    	   	SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
        //    SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);	
    		
    		
    		CNOT(Error_vector, 16, 2);
    		TQerror(Error_vector, 16, 2, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
        //    SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
    		
     		
     		CNOT(Error_vector, 16, 6);
     		TQerror(Error_vector, 16, 6, probability*beta);
    		SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
        //    SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
     		
     		
//flag 1c
			CNOT(Error_vector, 18, 4);
			TQerror(Error_vector, 18, 4, probability*beta);
 		
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
        //    SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
 		
 		
  			CNOT(Error_vector, 18, 6);
  			TQerror(Error_vector, 18, 6, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
        //    SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
  			
   			CNOT(Error_vector, 18, 8);
   			TQerror(Error_vector, 18, 8, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
        //    SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
   			
   			CNOT(Error_vector, 18, 7);
   			TQerror(Error_vector, 18, 7, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
        //    SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
            
  			CNOT(Error_vector, 18, 5);
  			TQerror(Error_vector, 18, 5, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
        //    SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);	
    	
    		CNOT(Error_vector, 18, 9);		
    		TQerror(Error_vector, 18, 9, probability*beta);
   			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
        //    SQerror(Error_vector, 9, probability*Gamma);
	
	
	        SQerror(Error_vector, 16, probability*Gamma);   
            SQerror(Error_vector, 17, probability*Gamma);
            SQerror(Error_vector, 18, probability*Gamma);	
	
	
	     	// ECC correct X error	
				// X1 error
                if (  Error_vector[0][9] == 1 && Error_vector[0][10]  == 0  ){
                    correct(Error_vector, 1, 2);
                    }
                // X2 error
                if (  Error_vector[0][9] == 1 && Error_vector[0][10]  == 1  ){
                    correct(Error_vector, 2, 2);
                    }
                // X3 error
                if (  Error_vector[0][9] == 0 && Error_vector[0][10]  == 1  ){
                    correct(Error_vector, 3, 2);
                    }
                // X4 error
                if (   Error_vector[0][11] == 1 && Error_vector[0][12]  == 0  ){
                    correct(Error_vector, 4, 2);
                    }
                // X5 error
                if (   Error_vector[0][11] == 1 && Error_vector[0][12]  == 1  ){
                    correct(Error_vector, 5, 2);
                    }
                // X6 error
                if (   Error_vector[0][11] == 0 && Error_vector[0][12]  == 1  ){
                    correct(Error_vector, 6, 2);
                    }
           
				// X7 error
				if (   Error_vector[0][13] == 1 && Error_vector[0][14]  == 0 ){
                    correct(Error_vector, 7, 2);
                    }
      
                // X8 error                
				if (   Error_vector[0][13] == 1 && Error_vector[0][14]  == 1 ){
                    correct(Error_vector, 8, 2);
                    }
                   
                // X9 error                
				if (   Error_vector[0][13] == 0 && Error_vector[0][14]  == 1 ){
                    correct(Error_vector, 9, 2);
                    }  		
					
       
                	// ECC correct Z error	
		
				if (Error_vector[1][15] == 1 && Error_vector[1][17] == 0 ){
				    correct(Error_vector, 1, 3);		
				}
				if (Error_vector[1][15] == 0 && Error_vector[1][17] == 1 ){
				    correct(Error_vector, 7, 3);	
				}	
			    if (Error_vector[1][15] == 1 && Error_vector[1][17] == 1 ){
				    correct(Error_vector, 4, 3);		
				}        		
							

}

int perfect(int (*Error_vector)[n]){
				
		//flag 1a

            CNOT(Error_vector, 1, 10); 
            CNOT(Error_vector, 2, 11);            
            CNOT(Error_vector, 4, 12);            
            CNOT(Error_vector, 5, 13);            
            CNOT(Error_vector, 7, 14);            
            CNOT(Error_vector, 8, 15);
            CNOT(Error_vector, 2, 10);           
            CNOT(Error_vector, 3, 11);         
            CNOT(Error_vector, 5, 12);           
            CNOT(Error_vector, 6, 13);         
            CNOT(Error_vector, 8, 14);                
            CNOT(Error_vector, 9, 15);
				
		//flag 1b
    
            CNOT(Error_vector, 16, 1);
            CNOT(Error_vector, 16, 17);
   			CNOT(Error_vector, 16, 3);
     		CNOT(Error_vector, 16, 5);
    		CNOT(Error_vector, 16, 4);
    		CNOT(Error_vector, 16, 2);
     		CNOT(Error_vector, 16, 17);
     		CNOT(Error_vector, 16, 6);
		//flag 1c
			CNOT(Error_vector, 18, 4);
 			CNOT(Error_vector, 18, 19);
  			CNOT(Error_vector, 18, 6);
   			CNOT(Error_vector, 18, 8);
   			CNOT(Error_vector, 18, 7);
  			CNOT(Error_vector, 18, 5);
    		CNOT(Error_vector, 18, 19);
    		CNOT(Error_vector, 18, 9);			
			

	     	// ECC correct X error	
				// X1 error
                if (  Error_vector[0][9] == 1 && Error_vector[0][10]  == 0  ){
                    correct(Error_vector, 1, 2);
                    }
                // X2 error
                if (  Error_vector[0][9] == 1 && Error_vector[0][10]  == 1  ){
                    correct(Error_vector, 2, 2);
                    }
                // X3 error
                if (  Error_vector[0][9] == 0 && Error_vector[0][10]  == 1  ){
                    correct(Error_vector, 3, 2);
                    }
                // X4 error
                if (   Error_vector[0][11] == 1 && Error_vector[0][12]  == 0  ){
                    correct(Error_vector, 4, 2);
                    }
                // X5 error
                if (   Error_vector[0][11] == 1 && Error_vector[0][12]  == 1  ){
                    correct(Error_vector, 5, 2);
                    }
                // X6 error
                if (   Error_vector[0][11] == 0 && Error_vector[0][12]  == 1  ){
                    correct(Error_vector, 6, 2);
                    }
           
				// X7 error
				if (   Error_vector[0][13] == 1 && Error_vector[0][14]  == 0 ){
                    correct(Error_vector, 7, 2);
                    }
      
                // X8 error                
				if (   Error_vector[0][13] == 1 && Error_vector[0][14]  == 1 ){
                    correct(Error_vector, 8, 2);
                    }
                   
                // X9 error                
				if (   Error_vector[0][13] == 0 && Error_vector[0][14]  == 1 ){
                    correct(Error_vector, 9, 2);
                    }  	      
	
				// ECC correct Z error	
		
				if (Error_vector[1][15] == 1 && Error_vector[1][17] == 0 ){
				    correct(Error_vector, 2, 3);		
				}
				if (Error_vector[1][15] == 0 && Error_vector[1][17] == 1 ){
				    correct(Error_vector, 5, 3);		
				}	
			    if (Error_vector[1][15] == 1 && Error_vector[1][17] == 1 ){
				    correct(Error_vector, 8, 3);		
				}
		
			
}


//---------------------------------------------------------------------------- 
int flag_rise1(int (*Error_vector)[n], double probability, double Gamma, double beta){
	

	
            SQerror(Error_vector, 50, probability);
            SQerror(Error_vector, 51, probability);
            SQerror(Error_vector, 52, probability);
			SQerror(Error_vector, 53, probability);
            SQerror(Error_vector, 54, probability);
            SQerror(Error_vector, 55, probability);	
	
	
	
 			CNOT(Error_vector, 1, 50);
            TQerror(Error_vector, 1, 50, probability*beta);
            CNOT(Error_vector, 2, 51);
            TQerror(Error_vector, 2, 51, probability*beta);
            CNOT(Error_vector, 4, 52);
            TQerror(Error_vector, 4, 52, probability*beta);
            CNOT(Error_vector, 5, 53);
            TQerror(Error_vector, 5, 53, probability*beta);
            CNOT(Error_vector, 7, 54);
            TQerror(Error_vector, 7, 54, probability*beta);           
            CNOT(Error_vector, 8, 55);
            TQerror(Error_vector, 8, 55, probability*beta);     
            SQerror(Error_vector, 3, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
            
            
            CNOT(Error_vector, 2, 50);
            TQerror(Error_vector, 2, 50, probability*beta);
            CNOT(Error_vector, 3, 51);
            TQerror(Error_vector, 3, 51, probability*beta);
            CNOT(Error_vector, 5, 52);
            TQerror(Error_vector, 5, 52, probability*beta);
            CNOT(Error_vector, 6, 53);
            TQerror(Error_vector, 6, 53, probability*beta);
            CNOT(Error_vector, 8, 54);
            TQerror(Error_vector, 8, 54, probability*beta);           
            CNOT(Error_vector, 9, 55);
            TQerror(Error_vector, 9, 55, probability*beta);     
            SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 7, probability*Gamma);     
            
            //--------------------------------------------
            
            SQerror(Error_vector, 50, probability);
            SQerror(Error_vector, 51, probability);
            SQerror(Error_vector, 52, probability);
			SQerror(Error_vector, 53, probability);
            SQerror(Error_vector, 54, probability);
            SQerror(Error_vector, 55, probability);	
		
} 

//---------------------------------------------------------------------------- 
int flag1a(int (*Error_vector)[n], double probability, double Gamma, double beta){
	
	
            SQerror(Error_vector, 10, probability);
            SQerror(Error_vector, 11, probability);
            SQerror(Error_vector, 12, probability);
			SQerror(Error_vector, 13, probability);
            SQerror(Error_vector, 14, probability);
            SQerror(Error_vector, 15, probability);	
	
	
	
            CNOT(Error_vector, 1, 10);
            TQerror(Error_vector, 1, 10, probability*beta);
            CNOT(Error_vector, 2, 11);
            TQerror(Error_vector, 2, 11, probability*beta);
            CNOT(Error_vector, 4, 12);
            TQerror(Error_vector, 4, 12, probability*beta);
            CNOT(Error_vector, 5, 13);
            TQerror(Error_vector, 5, 13, probability*beta);
            CNOT(Error_vector, 7, 14);
            TQerror(Error_vector, 7, 14, probability*beta);           
            CNOT(Error_vector, 8, 15);
            TQerror(Error_vector, 8, 15, probability*beta);     
            SQerror(Error_vector, 3, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);
            
            
            CNOT(Error_vector, 2, 10);
            TQerror(Error_vector, 2, 10, probability*beta);
            CNOT(Error_vector, 3, 11);
            TQerror(Error_vector, 3, 11, probability*beta);
            CNOT(Error_vector, 5, 12);
            TQerror(Error_vector, 5, 12, probability*beta);
            CNOT(Error_vector, 6, 13);
            TQerror(Error_vector, 6, 13, probability*beta);
            CNOT(Error_vector, 8, 14);
            TQerror(Error_vector, 8, 14, probability*beta);           
            CNOT(Error_vector, 9, 15);
            TQerror(Error_vector, 9, 15, probability*beta);     
            SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 7, probability*Gamma);     
            
            
            
            SQerror(Error_vector, 10, probability);
            SQerror(Error_vector, 11, probability);
            SQerror(Error_vector, 12, probability);
			SQerror(Error_vector, 13, probability);
            SQerror(Error_vector, 14, probability);
            SQerror(Error_vector, 15, probability);	
		
   
	
}

int flag1b(int (*Error_vector)[n], double probability, double Gamma, double beta){ 


		    SQerror(Error_vector, 16, probability);
            SQerror(Error_vector, 18, probability);
            SQerror(Error_vector, 19, probability);
            
            
              
            
            CNOT(Error_vector, 16, 1);
            TQerror(Error_vector, 16, 1, probability*beta);
            
           
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);   
           
            SQerror(Error_vector, 18, probability*Gamma);
            SQerror(Error_vector, 19, probability*Gamma);  

        //===============================================================
        


            CNOT(Error_vector, 18, 7);
            TQerror(Error_vector, 18, 7, probability*beta);
               


            CNOT(Error_vector, 16, 19);
            TQerror(Error_vector, 16, 19, probability*beta);
            
            
            SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   

            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);   
       
            
            
        //===============================================================
        

            
       		CNOT(Error_vector, 18, 19);
            TQerror(Error_vector, 18, 19, probability*beta);
      		CNOT(Error_vector, 16, 3);
            TQerror(Error_vector, 16, 3, probability*beta);
            
            SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);

            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);   
             
            
            
        //===============================================================
        

            
            CNOT(Error_vector, 18, 9);
            TQerror(Error_vector, 18, 9, probability*beta);
      
			CNOT(Error_vector, 16, 5);
            TQerror(Error_vector, 16, 5, probability*beta);
            
			SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);

            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);


            SQerror(Error_vector, 19, probability*Gamma);             
            
            
        
         //===============================================================
        


			CNOT(Error_vector, 18, 5);
            TQerror(Error_vector, 18, 5, probability*beta);
            
			CNOT(Error_vector, 16, 4);
            TQerror(Error_vector, 16, 4, probability*beta);
          
          
            SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
         
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);   

            SQerror(Error_vector, 19, probability*Gamma);  
          
    	//===============================================================
        

			
			CNOT(Error_vector, 18, 8);
            TQerror(Error_vector, 18, 8, probability*beta);
				        
			CNOT(Error_vector, 16, 2);
            TQerror(Error_vector, 16, 2, probability*beta);
            
            SQerror(Error_vector, 1, probability*Gamma);

            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);

            SQerror(Error_vector, 9, probability*Gamma);   

            SQerror(Error_vector, 19, probability*Gamma);             
            
            
     	//===============================================================
        

                              
			CNOT(Error_vector, 18, 4);
            TQerror(Error_vector, 18, 4, probability*beta);
     		                        

			CNOT(Error_vector, 16, 19);
            TQerror(Error_vector, 16, 19, probability*beta);
            
            SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   

            SQerror(Error_vector, 5, probability*Gamma);
            SQerror(Error_vector, 6, probability*Gamma);   
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);   
         
            
            
        //===============================================================
        

          
			CNOT(Error_vector, 18, 19);
            TQerror(Error_vector, 18, 19, probability*beta);
            
			CNOT(Error_vector, 16, 6);
            TQerror(Error_vector, 16, 6, probability*beta);

 
            SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
  
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);   
 
            
  		//===============================================================
        


			CNOT(Error_vector, 18, 6);
            TQerror(Error_vector, 18, 6, probability*beta);
 
            SQerror(Error_vector, 1, probability*Gamma);
            SQerror(Error_vector, 2, probability*Gamma);
            SQerror(Error_vector, 3, probability*Gamma);   
            SQerror(Error_vector, 4, probability*Gamma);
            SQerror(Error_vector, 5, probability*Gamma);
          
            SQerror(Error_vector, 7, probability*Gamma);
            SQerror(Error_vector, 8, probability*Gamma);
            SQerror(Error_vector, 9, probability*Gamma);   
            SQerror(Error_vector, 16, probability*Gamma);

            SQerror(Error_vector, 19, probability*Gamma);            
	
  		//===============================================================


   		    SQerror(Error_vector, 16, probability);
            SQerror(Error_vector, 18, probability);
            SQerror(Error_vector, 19, probability);

   
     
		 
           
} 

