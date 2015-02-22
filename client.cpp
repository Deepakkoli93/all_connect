#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <bits/stdc++.h>
/* Added Code */
#include <time.h>
#include <sys/time.h>
/* */

#define FOR(i,n) for (int i = 0; i < n; i++)
#define PB push_back
#define GAMMA 0.99999
#define EPS 0.00000000001
const int max_columns = 100;
static int ninfinity = -32766;
static int pinfinity = 32767;
using namespace std;
/* Complete the function below to print 1 integer which will be your next move 
   */
int getutility(vector<string> b);
int N, M, K, player;
/* Added Code */
double time_left = 0;
/* */
char mymove = (player == 1)?'X':'O';
char hismove = (player == 1)?'O':'X';

vector<string> board;
struct Node
{ 
  vector<string> state; 
  string level; //max or min
  int value; //the utility
  int action; //the column number of the move
  int alpha ;
  int beta ;
  //Node *successors[max_columns];
  vector<Node*> successors;
  Node* parent; 
};
int counter = 0;
int turn_number = 0;
void update(int n)
{
    for(int i = N-1;i >=0 ; i--)
        if(board[i][n] == '.')
        {   if(((counter++) % 2) == 0)
            board[i][n] = 'X';
            else
            board[i][n] = 'O';
            return;
        }
}

/*void update(int n, int player)
{
    for(int i = 0; i < N; i++)
        if(board[i][n] == '.')
        {
            if(player == 1)
              board[i][n] = 'X';
            else
              board[i][n] = 'O';
            return;
        }
}*/


//will be used for visualizing intermediate boards, also takes X or O.
vector<string> updated_board(Node* node, int n, char c){
  vector<string> b = (*node).state;
  for(int i=N-1; i>=0; i--){
    if(b[i][n] == '.')
      {b[i][n] = c;
        /*for(int i=0;i<N;i++){
          cout<<b[i]<<endl;
        }
        cout<<"\n";*/
       return b;
      }
  }
}

vector<int> available_columns(Node* node){

 //0 is unavailable
 //1 is available
  vector<int> columns(M,0);
  vector<string> b = (*node).state; 
  
  //int columns[M] = {0};
//cout<<"in available_columns..printing...\n";
/*for(int i=0;i<N;i++)
cout<<endl<<b[i]<<endl;*/

  for(int j=0; j<M; j++){
    for(int i=0; i<N; i++){
       if(b[i][j] == '.')
       {
        columns[j]++;
        break;
       }
    }
  }
  //cout<<endl<<"available columns are"<<endl;
  /*for(int i=0;i<M;i++)
    cout<<columns[i];
   cout<<endl;*/
return columns;
}
// pass a node to it & it will return the node with 
//a minimax tree till specified depth
void generate_minimax(Node* node , int depth){
  if(depth == 0 ) 
    {for(int i=0;i<M;i++){
      (*node).successors.push_back(NULL);
    }
      //return node;
  }
    
  else {
    for(int i=0;i<M;i++){
      (*node).successors.push_back(NULL);
    }
   vector<int> columns = available_columns(node);
  //cout <<"printing all the possible moves... \n"; 
 for(int i=0;i<M;i++){
  if(columns[i] == 1){// if a move can be played into that column
     //CHANGE THIS ACCORDING TO THE PLAYER
    vector<string> newstate;
   if(((*node).level) == "max")
   newstate = updated_board(node,i,mymove);
   else if (((*node).level) == "min")
   newstate = updated_board(node,i,hismove);
   Node* n = new Node;
   (*node).successors[i] = n;
   (*node).successors[i] -> state = newstate;
   (*node).successors[i] -> level = ((node -> level == "max")? "min":"max");
   (*node).successors[i] -> parent = node;
   (*node).successors[i] -> action = i; // column number of the move
  

   
   //generate_minimax((*node).successors[i] ,depth-1);
   //return node;
                    }
                    }  
   //cout<<"possible moves have been printed, now going ahead... \n";    
   for(int i=0; i< M ;i++){
    if(columns[i] == 1)
      generate_minimax((*node).successors[i] ,depth-1);
   }                      
    //return node ;   
       }

                                           } 
//terminal test
bool isterminal(Node* node){
  for(int i=0;i<M;i++) {
     if(node -> successors[i] != NULL)
      return false;
  }
  return true;
}

int max_val(Node* node){
  int max = -65000;
  for(int i=0;i<M;i++){
    if((*node).successors[i] != NULL)
    if((*node).successors[i] -> value > max)
     max = (*node).successors[i] -> value;
   
  }
  //cout<<"max is = "<<max<<"\n";
  return max;
}
int min_val (Node* node) {
  int min = 65000;
  for(int i=0;i<M;i++) {
    if(node -> successors[i] != NULL)
    if(node -> successors[i] -> value < min)
      min = node -> successors[i] -> value;
  }
  //cout<<"min is = "<<min<<"\n";
  return min;
}
//pass a node to it and it will return the node with
//backed up values

void backup_values(Node* node){
if(isterminal(node))
{  
   //cout<<"\n is a terminal node \n";
   (*node).value = getutility((*node).state);
   //cout<<"got utility from terminal nodes = "<<(*node).value<<"\n";
   //return node;
}
else {

  //cout<<"\n is not a terminal node \n";
  if((*node).level == "max")
    {for(int i=0;i<M;i++){
      if((*node).successors[i] != NULL)
      backup_values((*node).successors[i]);
    }
      (*node).value = max_val(node);
    }
    
  else if ((*node).level == "min")
    {for(int i=0;i<M;i++){
      if((*node).successors[i] != NULL)
      backup_values((*node).successors[i]);
    }
      (*node).value = min_val(node);
    }

}
//return node;
}
pair<int,int> score_client(vector <string> b, int inaline=K){
 int arr[2] = {0,0};
 int k = inaline;
 int i,j=0;
 //cout<<"\n";
 /*for(i=0;i<N;i++)
 {
  
    cout<<b[i];
  cout<<"\n";
 }*/
  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      if(b[i][j] == '.')
        continue;
      int l_k=0;
      //horizontally
      //cout<<"checking horizontally!!";
      for(l_k=0; l_k<k && j+l_k<M; l_k++)
      { 
        if(b[i][j]!=b[i][j+l_k])
          {//cout<<"l_k = "<<l_k<<"\n";
           //cout<<"( "<<i<<","<<j<<" ) = ("<<i<<","<<j+l_k<<" ) \n";
            break;}
      }
      if(l_k == k) 
        {if(b[i][j] == 'X')
          { //cout<<"thats an X \n ("<<i<<","<<j<<") to ("<<i<<","<<j+l_k<<") \n";
            arr[0]++;}
         else if (b[i][j] == 'O')
          {//cout<<"thats an X \n ("<<i<<","<<j<<") to ("<<i+l_k<<","<<j<<") \n";
        arr[1]++;}}
     

      //vertically
      l_k=0;
      //cout<<"checking vertically";
      for(l_k=0; l_k<k && i+l_k<N; l_k++)
      { 
        if(b[i][j]!=b[i+l_k][j])
          {//cout<<"l_k = "<<l_k<<"\n";
           //cout<<"( "<<i<<","<<j<<" ) = ("<<i+l_k<<","<<j<<" ) \n";
            break;}
      }
      if(l_k == k)
        {if(b[i][j] == 'X')
          {//cout<<"thats an X \n ("<<i<<","<<j<<") to ("<<i+l_k<<","<<j<<") \n";
          arr[0]++;}
         else if (b[i][j] == 'O')
          {//cout<<"thats an O \n ("<<i<<","<<j<<") to ("<<i+l_k<<","<<j<<") \n";
        arr[1]++;}}

     //diagonally upwards
        l_k = 0;
        for(l_k=0;l_k<k && ((i-l_k)>=0) && ((j+l_k)<M);l_k++)
        {
          if(b[i][j] != b[i - l_k][j + l_k])
            break;
        }
        
        if(l_k == k)
        {
          if(b[i][j] == 'X')
            arr[0]++;
          else if (b[i][j] == 'O')
            arr[1]++;
        }

      //diagonally downwards
        l_k=0;
        for(l_k=0;l_k<k && ((i+l_k)<N) && ((j+l_k)<M);l_k++)
        {
          if(b[i][j] != b[i + l_k][j + l_k])
            break;
        }
        
        if(l_k == k)
        {
          if(b[i][j] == 'X')
            arr[0]++;
          else if (b[i][j] == 'O')
            arr[1]++;
        }
    }
  }
return make_pair(arr[0],arr[1]);


}
//---------------xxxxxxxxxxxxxxxxxxxxxxxxxxx-----------------------------
//second utility function that takes into account if a player is close to 
//connecting k
//---------------xxxxxxxxxxxxxxxxxxxxxxxxxxx-----------------------------
//helper function for the following heuristic function
bool huhelper(vector <string> b, int i, int j, int l_k, int rest, int c)
{
  
  switch(c)
  {
    case 1: 
    for(int z=1; z<=rest; z++)
    {
      if(b[i][j+l_k+z] == '.')
        continue;
      else
        return false;
    }
    return true;

    case 2:
    for(int z=1; z<=rest; z++)
    {
      if(b[i+l_k+z][j] == '.')
        continue;
      else
        return false;
    }
    return true;

    case 3:
    for(int z=1; z<=rest; z++)
    {
      if(b[i-l_k-z][j+l_k+z] == '.')
        continue;
      else
        return false;
    }
    return true;

    case 4:
    for(int z=1; z<=rest; z++)
    {
      if(b[i+l_k+1][j+l_k+1] == '.')
        continue;
      else
        return false;
    }
    return true;
  }
}
//---------------xxxxxxxxxxxxxxxxxxxxxxxxxxx-----------------------------


//how may are there in a line ---- inaline
//how many to check further --- rest
pair<int,int> heuristic_2(vector <string> b, int inaline, int rest){
 int arr[2] = {0,0};
 int k = inaline-1;
 int i,j=0;
 //cout<<"\n";
 /*for(i=0;i<N;i++)
 {
  
    cout<<b[i];
  cout<<"\n";
 }*/
  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      if(b[i][j] == '.')
        continue;
      int l_k=0;
      //horizontally
      //cout<<"checking horizontally!!";
      for(l_k=0; l_k<k && j+l_k<M-rest; l_k++)
      { 
        if(b[i][j]!=b[i][j+l_k])
          {//cout<<"l_k = "<<l_k<<"\n";
           //cout<<"( "<<i<<","<<j<<" ) = ("<<i<<","<<j+l_k<<" ) \n";
            break;
          }
      }
      if((j+l_k) < M-rest)
       //if( (l_k == k) && (b[i][j+l_k+1] == '.') ) 
       if( (l_k == k) && huhelper(b,i,j,l_k,rest,1) ) 
        {if(b[i][j] == 'X')
          { //cout<<"thats an X \n ("<<i<<","<<j<<") to ("<<i<<","<<j+l_k<<") \n";
            arr[0]++;
          }
         else if (b[i][j] == 'O')
          {//cout<<"thats an X \n ("<<i<<","<<j<<") to ("<<i+l_k<<","<<j<<") \n";
        arr[1]++;
          }
        }
     

      //vertically
      l_k=0;
      //cout<<"checking vertically";
      for(l_k=0; l_k<k && i+l_k<N-rest; l_k++)
      { 
        if(b[i][j]!=b[i+l_k][j])
          {//cout<<"l_k = "<<l_k<<"\n";
           //cout<<"( "<<i<<","<<j<<" ) = ("<<i+l_k<<","<<j<<" ) \n";
            break;
          }
      }
      if( (i+l_k) < N-rest )
       if( (l_k == k) && huhelper(b,i,j,l_k,rest,2) )
        {if(b[i][j] == 'X')
          {//cout<<"thats an X \n ("<<i<<","<<j<<") to ("<<i+l_k<<","<<j<<") \n";
          arr[0]++;
        }
         else if (b[i][j] == 'O')
          {//cout<<"thats an O \n ("<<i<<","<<j<<") to ("<<i+l_k<<","<<j<<") \n";
        arr[1]++;
          }
      } 

     //diagonally upwards
        l_k = 0;
        for(l_k=0;l_k<k && ((i-l_k)>=rest) && ((j+l_k)<M-rest);l_k++)
        {
          if(b[i][j] != b[i - l_k][j + l_k])
            break;
        }
        
        if( ((i-l_k)>=rest) && ((j+l_k)<M-rest) )
        if((l_k == k) && huhelper(b,i,j,l_k,rest,3) )
        {
          if(b[i][j] == 'X')
            arr[0]++;
          else if (b[i][j] == 'O')
            arr[1]++;
        }

      //diagonally downwards
        l_k=0;
        for(l_k=0;l_k<k && ((i+l_k)<N-rest) && ((j+l_k)<M-rest);l_k++)
        {
          if(b[i][j] != b[i + l_k][j + l_k])
            break;
        }
        
        if(((i+l_k)<N-rest) && ((j+l_k)<M-rest)) 
        if( (l_k == k) && huhelper(b,i,j,l_k,rest,4))
        {
          if(b[i][j] == 'X')
            arr[0]++;
          else if (b[i][j] == 'O')
            arr[1]++;
        }
    }
  }
return make_pair(arr[0],arr[1]);


}



//---------------xxxxxxxxxxxxxxxxxxxxxxxxxxx-----------------------------
//pass a board to get its utility function
int getutility (vector <string> b){

pair <int,int> res1= score_client(b);
pair <int,int> res2= heuristic_2(b,K-1,1);
pair <int,int> res3= heuristic_2(b,K-2,2);
pair <int,int> res4= heuristic_2(b,K-3,3);
//pair <int,int> res5=heuristic_2(b,K-4,4);
//cout<<"res.first = "<<res.first<<"\n";
//cout<<"res.second = "<<res.second<<"\n";
//return (res.first - res.second);
int p1 = (res1.first*16) + (res2.first*8) + (res3.first*2) + (res4.first) ;
int p2 = (res1.second*16) + (res2.second*8) + (res3.second*2) + (res4.second);
/*if(player == 2)
{cout<<"I am player 2";return (p2-p1);}
else if (player == 1)
{cout<<"I am player 1";return (p2-p1);}*/
return (p2-p1);
}

void print_board(vector <string> b) {
  for(int i=0;i<N;i++)
    cout<<b[i]<<"\n";
  cout<<"\n";
  cout<<"\n";
}

void print_tree(Node* node, int depth){
  cout<<"\n............level "<<depth<<"..............\n";
  for(int i=0; i<M ; i++){
    if((*node).successors[i] != NULL)
      {print_board((*node).successors[i] -> state);
        cout<<"Value: "<<(*node).successors[i] -> value<<"\n";
       
     }
  }
  for(int i=0; i<M ;i++){
    if((*node).successors[i] != NULL)
    {
      print_tree((*node).successors[i], depth -1);
    }
  }

}
bool isvalidmove(int column)
{
  for(int i=N-1; i>=0; i--)
  {
    if(board[i][column] == '.')
      return true;
    else
      continue;
  }
  return false;
}
int max_of_two(int a, int b)
{
  return  ((a>b)?a:b);
}
int min_of_two(int a, int b)
{
  return ((a<b)?a:b);
}
//attempt to implement alpha beta pruning 

int alphabetapruning(Node* node, int depth, int alpha, int beta,bool maxplayer){
  if(depth == 0 || isterminal(node))
  {
    float thevalue = getutility((*node).state); 
    node -> value = thevalue;
    //return getutility((*node).state);
    return thevalue;
  }
  if(maxplayer)
  {
    for(int i=0;i<M;i++)
    {
      if((*node).successors[i] != NULL)
      {
        alpha = max_of_two(alpha,alphabetapruning((*node).successors[i],depth - 1, alpha, beta, false));
        if( beta <= alpha)
          break;

      }
    }
    node -> value = alpha;
    return alpha;
  }
  else
  {
    for(int i=0;i<M; i++)
    {
      if((*node).successors[i] != NULL)
      {
        beta = min_of_two(beta, alphabetapruning((*node).successors[i],depth - 1,alpha, beta, true));
        if(beta <= alpha)
         break;
      }
    }
    node -> value = beta;
    return beta;
  }
}

int nextMove()
{

//prev_inline=score_client(board);

turn_number+=2;//this is the turn number i am going to move now
  /*for(int j = 0; j< M; j++)
  {
    for(int i = 0; i<N; i++)
      if(board[i][j] == '.')
      {
           board[i][j] = 'O';
           cout << j << endl;
        //return j;
   
  }}*/
  
  //int x,y;
//a hack to invert the board
//cout<<"---------------- I am player number "<<player<<"------------";
vector<string> board2;
string temp2 = "";
    for(int j = 0; j< M; j++)
        temp2 += ".";
    FOR(i, N)
        board2.PB(temp2);           

int i,j;
for(i=0,j=N-1;i<N;i++,j--){
 board2[i] = board[j];
}           

//cout << "\n------ cuurent state of the board is ------\n";
//print_board(board);
//cout << "\n ------------------------------------------\n";
Node* node = new Node;
(*node).state = board;
(*node).level = "max";
for(int i=0;i<M;i++)
 (*node).successors.push_back(NULL); 
//(*node).successors(M,NULL);
Node* node_with_values = new Node;

if((N<=8)&&(M<=9))
{
if(player==1)
{
 generate_minimax(node,4);
 
//backup_values(node);
//print_tree(node,3);
alphabetapruning(node,4,ninfinity, pinfinity, true) ; 
}

else{
if(turn_number<(N*M/4+4)){
generate_minimax(node,5);
 
//backup_values(node);
//print_tree(node,3);
alphabetapruning(node,5,ninfinity, pinfinity, true) ;}

else if ((turn_number>=(N*M/4+4))&&(turn_number<(N*M/2))){
  //cout<<"got here1\n";
generate_minimax(node,5);
 
//backup_values(node);
//print_tree(node,3);
alphabetapruning(node,5,ninfinity, pinfinity, true) ;}
else if ((turn_number>=(N*M/2))&&(turn_number<(3*N*M/4)))
{
  //cout<<"got here2\n";
generate_minimax(node,5);
 
//backup_values(node);
//print_tree(node,3);
alphabetapruning(node,5,ninfinity, pinfinity, true) ;
}
else{
  //cout<<"got here3\n";
generate_minimax(node,6);
 
//backup_values(node);
//print_tree(node,3);
alphabetapruning(node,6,ninfinity, pinfinity, true) ;
}

}
}
else if ((N==10)&&(M==11)){
 if(player==1){ generate_minimax(node,4);
 
//backup_values(node);
//print_tree(node,3);
alphabetapruning(node,4,ninfinity, pinfinity, true); }
else{
  if(turn_number<(N*M/4+4)){
generate_minimax(node,3);
 
//backup_values(node);
//print_tree(node,3);
alphabetapruning(node,3,ninfinity, pinfinity, true) ;}

else if ((turn_number>=(N*M/4+4))&&(turn_number<(N*M/2))){
  //cout<<"got here1\n";
generate_minimax(node,4);
 
//backup_values(node);
//print_tree(node,3);
alphabetapruning(node,4,ninfinity, pinfinity, true) ;}
else if ((turn_number>=(N*M/2))&&(turn_number<(3*N*M/4)))
{
  //cout<<"got here2\n";
generate_minimax(node,5);
 
//backup_values(node);
//print_tree(node,3);
alphabetapruning(node,5,ninfinity, pinfinity, true) ;
}
else{
  //cout<<"got here3\n";
generate_minimax(node,5);
 
//backup_values(node);
//print_tree(node,3);
alphabetapruning(node,5,ninfinity, pinfinity, true) ;
}


}
}
else{
  generate_minimax(node,4);
 
//backup_values(node);
//print_tree(node,3);
alphabetapruning(node,4,ninfinity, pinfinity, true) ;
}
node_with_values = node;

int value_at_node = (*node_with_values).value;
int answer = 0;
/*cout<<"\n successors values are.. \n";
for(int i=0;i<M;i++){
  if((*node_with_values).successors[i] != NULL)
    cout<<(*node_with_values).successors[i] -> value<<endl;
}*/

//cout<<"value at the node is = "<<value_at_node;
for(int i=0;i<M;i++){
  if((*node_with_values).successors[i] != NULL)
    if((*node_with_values).successors[i] -> value == value_at_node)
      {
        answer = (*node_with_values).successors[i] -> action;
        if(isvalidmove(answer))
          {
            //cout<<"\n Move is valid\n";
            break;
          }
        else
          continue;
      }
}

//cout<<"\n Move is column number = "<<answer<<"\n";
 /*for(int i =0; i<M;i++){
}
for(int qq =0;qq<N;qq++){
cout<< board[i][qq] <<" ";
}
cout<<"\n";}*/
  //cout<<"entered nextmove!!";
  /*pair<int,int> res = score_client();
  
  cout<<"\n score() completed!!!!";
  
  cout<<"\n current score: \n";
  cout<<res.first<<" "<<res.second<<"\n";*/
    /*int iSecret;
    vector<int> v;
  for(int j = 0; j< M; j++)
  {
    //for(int i = 0; i<N; i++)
      if(board[N-1][j] == '.')
      {
        //return j;
        //cout << j << endl;
        v.push_back(j);
      }
  }
  int temp = rand();
  iSecret = temp%v.size();*/
  //cout << temp << " " << iSecret << " " << v[iSecret] << endl;
  //cout<<"isecret "<<v[iSecret];
  //update(v[iSecret]);
  update(answer);
  //turn_number++;
  //cout<<"move in : "<<answer<<" at my turn number: "<<turn_number<<"\n";
  pair<int,int> res = score_client(board);
  //cout<<"\n score() completed!!!!";
  
  //cout<<"\n current score: \n";
  //cout<<res.first<<" "<<res.second<<"\n";
  //return v[iSecret];
  //cout<<"time left = "<<time_left<<"\n";
  return answer;
}




int main(int argc, char *argv[])
{
    srand (time(NULL));
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1025];
    struct sockaddr_in serv_addr; 

    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> <port no> \n",argv[0]);
        return 1;
    } 

    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    cout<<"Connect(N,M,K) will start"<<endl;

    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d", &N);

    cout<<"N: "<<N<<endl;
    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d", &M);

    cout<<"M: "<<M<<endl;
    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d", &K);    
    
    cout<<"K: "<<K<<endl;

/* Added code */
    int TL;
    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d", &TL);    
    
    cout<<"Time Limit: "<<TL<<endl;    
    time_left = TL;

/* */

    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d", &player);  

    cout<<"Player "<<player<<endl;

    string temp = "";
    for(int j = 0; j< M; j++)
        temp += ".";
    FOR(i, N)
        board.PB(temp);

    if(player == 1)
    {
        //cout << "hello" << endl;
        memset(sendBuff, '0', sizeof(sendBuff)); 
        int temp = nextMove();
        //cout << "temp" << endl;
        snprintf(sendBuff, sizeof(sendBuff), "%d", temp);
        write(sockfd, sendBuff, strlen(sendBuff));
    }

    while(1)
    {
        int nextColumn;

        memset(recvBuff, '0',sizeof(recvBuff));
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);

        /* Added Code */
        struct timeval  tv1, tv2;
        gettimeofday(&tv1, NULL);
        /* */

        recvBuff[n] = 0;
        sscanf(recvBuff, "%d", &nextColumn);
        //cout << "Rec: "<< nextColumn << endl;
        if(player == 1)
             {//update(nextColumn, 2);
                update(nextColumn);}
        else 
          {//update(nextColumn, 1);
          update(nextColumn);}

        cout << nextColumn << endl;
        
        memset(sendBuff, '0', sizeof(sendBuff)); 
        int temp = nextMove();
        //cout << "temp" << endl;
        snprintf(sendBuff, sizeof(sendBuff), "%d", temp);

        /** Added Code */
        gettimeofday(&tv2, NULL);
        double time_spent = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
        time_left -= time_spent;
        /* */

        write(sockfd, sendBuff, strlen(sendBuff));
    }

    return 0;
}
