#include<unistd.h>
#include <stdio.h>
#include<sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<stdbool.h>
#include<fcntl.h>
#include<string.h>

typedef struct normalUser{
	
	int userId;	
	char username[30];
	char password[30];
	long int accountNumber;
	double balance;
	int status;

}normalUser;

typedef struct Admin{
	
	int userId;
	char username[30];
	char password[30];
	int status;
}admin;

typedef struct jointUser{

	int userId;
	char username1[30];
	char username2[30];
	char password[30];
	long int accountNumber;
	double balance;
	int status;
}jointUser;



void updateDataN(normalUser a);
void updateDataA(admin a);
void updateDataJ(jointUser a);

jointUser readDataJ(jointUser a);
normalUser readDataN(normalUser a);
admin readDataA(admin a);

int validateAdmin(admin a){
		
	admin a1;
	int fd = open("Admin.txt",O_RDONLY);

	struct flock lock;				//initialising the lock.
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (a.userId)*sizeof(admin);	//lock starts from the starting of the required record.
	//lock.l_start = recordNum;	
	lock.l_len = sizeof(admin);
	lock.l_pid = getpid();

	
	int k=fcntl(fd,F_SETLKW,&lock);	

	
	lock.l_type = F_UNLCK;
	int j=fcntl(fd,F_SETLK,&lock);

	lseek(fd,a.userId*(sizeof(admin)),SEEK_SET);
	read(fd,&a1,sizeof(admin));

	if(strcmp(a1.password,a.password)==0){
		printf("ValidUser\n");		
		return 1;	
	}
	printf("invalidUser\n");

	
	return 0;

}

int validatejointUser(jointUser u){

	jointUser u1;
	int fd = open("Joint.txt",O_RDONLY);
	
	struct flock lock;				//initialising the lock.
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (u.userId)*sizeof(jointUser);	//lock starts from the starting of the required record.
	//lock.l_start = recordNum;	
	lock.l_len = sizeof(jointUser);
	lock.l_pid = getpid();

	int k=fcntl(fd,F_SETLKW,&lock);
	lseek(fd,u.userId*(sizeof(jointUser)),SEEK_SET);
	read(fd,&u1,sizeof(jointUser));
	
	lock.l_type = F_UNLCK;
	int j=fcntl(fd,F_SETLK,&lock);


	close(fd);

	if(strcmp(u1.password,u.password)==0){
		printf("ValidUser\n");		
		return 1;	
	}
	printf("invalidUser\n");
	return 0;
}

int validatenUser(normalUser a){

	normalUser a1;
	int fd = open("normalUsers.txt",O_RDONLY);
	
	struct flock lock;				//initialising the lock.
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (a.userId)*sizeof(normalUser);	//lock starts from the starting of the required record.
	//lock.l_start = recordNum;	
	lock.l_len = sizeof(normalUser);
	lock.l_pid = getpid();

	
	int k=fcntl(fd,F_SETLKW,&lock);

	lseek(fd,a.userId*(sizeof(normalUser)),SEEK_SET);
	read(fd,&a1,sizeof(normalUser));
	
	lock.l_type = F_UNLCK;
	int j=fcntl(fd,F_SETLK,&lock);

	close(fd);

	if(strcmp(a1.password,a.password)==0 && a1.status==1){
		printf("ValidUser\n");		
		return 1;	
	}
	printf("invalidUser\n");
	
	
	return 0;
}

normalUser readLastN(){

	normalUser a;
	int fd = open("normalUsers.txt",O_RDONLY);
	

	lseek(fd,-1*(sizeof(normalUser)),SEEK_END);
	read(fd,&a,sizeof(a));	
	
	
	close(fd);	
	return a;
}

jointUser readLastJ(){
	jointUser a;
	int fd = open("Joint.txt",O_RDONLY);
	lseek(fd,-1*(sizeof(jointUser)),SEEK_END);
	read(fd,&a,sizeof(a));	
	close(fd);	
	return a;
} 

normalUser readByIdN(int userId){

	normalUser a;	
	int fd = open("normalUsers.txt",O_RDONLY);
	struct flock lock;				//initialising the lock.
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (userId)*sizeof(normalUser);	//lock starts from the starting of the required record.
	//lock.l_start = recordNum;	
	lock.l_len = sizeof(normalUser);
	lock.l_pid = getpid();

	int k=fcntl(fd,F_SETLKW,&lock);

	lseek(fd,userId*(sizeof(normalUser)),SEEK_SET);
	read(fd,&a,sizeof(a));

	lock.l_type = F_UNLCK;
	int j=fcntl(fd,F_SETLK,&lock);	

	close(fd);
	return a;
} 

jointUser readByIdJ(int userId){
	jointUser a;
	int fd = open("JointUsers.txt",O_RDONLY);
	
	struct flock lock;				//initialising the lock.
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (userId)*sizeof(jointUser);	//lock starts from the starting of the required record.
	//lock.l_start = recordNum;	
	lock.l_len = sizeof(jointUser);
	lock.l_pid = getpid();

	int k=fcntl(fd,F_SETLKW,&lock);


	lseek(fd,userId*(sizeof(jointUser)),SEEK_SET);
	read(fd,&a,sizeof(a));

	lock.l_type = F_UNLCK;
	int j=fcntl(fd,F_SETLK,&lock);	

	close(fd);
	return a;
}


int addAccount(int nsd,int type){

	if(type==1){
		normalUser nU;
		read(nsd,&nU,sizeof(normalUser));
		nU.balance = 0;
		normalUser lastuser=readLastN();
		nU.userId = lastuser.userId+1;
		nU.userId = lastuser.accountNumber+1;
		nU.status = 1;
		updateDataN(nU);
	}
	
	else if(type==2){
		jointUser jU;
		read(nsd,&jU,sizeof(jointUser));
		jU.balance = 0; 		
		jointUser lUser = readLastJ();
		jU.userId = lUser.userId+1;
		jU.accountNumber = lUser.accountNumber+1;
		jU.status=1;
		updateDataJ(jU);
	}

	return 1;
}

int deleteAccount(int nsd,int type){

	int userId;
	read(nsd,&userId,sizeof(userId));
	if(type==1){
		normalUser a;
		a = readByIdN(userId);
		if(a.status==1){
			a.status=0;
			updateDataN(a);			
			return 1;}
	}	
	else if(type==2){
		jointUser a;
		a = readByIdJ(userId);
		if(a.status==1){
			a.status=0;
			updateDataJ(a);return 1;
		}
	}
	return 0;
}

int modifyAccount(int nsd,int type){

	if(type==1){
		normalUser a;
		read(nsd,&a,sizeof(normalUser));
		updateDataN(a);
	}	
	else if(type==2){
		jointUser a;
		read(nsd,&a,sizeof(jointUser));updateDataJ(a);
	}

	return 1;
}

int searchAccount(int nsd,int type){
	int userId;
	read(nsd,&userId,sizeof(userId));
	if(type==1){
		normalUser a;
		a = readByIdN(userId);
		write(nsd,&a,sizeof(a));
	}
	
	else if(type==2){
		jointUser a; 
		a = readByIdJ(userId);
		write(nsd,&a,sizeof(a));
	}
	
	return 1;
}

/*void updateDataN(normalUser a);
void updateDataA(admin a);
void updateDataJ(jointUser a);
*/

int DepositN(int nsd,normalUser u){

	u = readDataN(u);

	int amount;
	
	read(nsd,&amount,sizeof(amount));
	
	u.balance += amount;
	
	//printf("Balance Added : %lf\n",u.balance);
	
	updateDataN(u);

	return 1;
}


int WithdrawN(int nsd,normalUser u){

	u = readDataN(u);	
	
	int amount;

	read(nsd,&amount,sizeof(amount));
		
	if(u.balance < amount)return 0;

	u.balance -= amount;
	
	//printf("Balance Now : %lf\n",u.balance);

	updateDataN(u);
	
	return 1;
}


void BalanceEnquiryN(int nsd,normalUser u){

	//u.balance=0;
	u = readDataN(u);
	int balance = u.balance;	
	write(nsd,&balance,sizeof(balance));
}


int PasswordChangeN(int nsd,normalUser u){

	u = readDataN(u);
	read(nsd,&u.password,sizeof(u.password));
	updateDataN(u);
	return 1;
}

void viewDetailsN(int nsd,normalUser u){

	u=readDataN(u);
	write(nsd,&u,sizeof(u));

}

int DepositJ(int nsd,jointUser u){

	int amount;

	u=readDataJ(u);	

	read(nsd,&amount,sizeof(amount));
	
	u.balance += amount;
	
	printf("Balance Added : %lf\n",u.balance);
	
	updateDataJ(u);

	return 1;
}


int WithdrawJ(int nsd,jointUser u){

	int amount;
	
	u=readDataJ(u);

	read(nsd,&amount,sizeof(amount));
		
	if(u.balance < amount)return 0;

	u.balance -= amount;
	
	//printf("Balance Now : %lf\n",u.balance);

	updateDataJ(u);
	
	return 1;
}


void BalanceEnquiryJ(int nsd,jointUser u){

	u = readDataJ(u);
	//u.balance=0;
	int balance = u.balance;	
	write(nsd,&balance,sizeof(balance));
}


int PasswordChangeJ(int nsd,jointUser u){

	u = readDataJ(u);
	read(nsd,&u.password,sizeof(u.password));
	//printf("New Password : %s\n",u.password);
	updateDataJ(u);
	return 1;
}

void viewDetailsJ(int nsd,jointUser u){

	u = readDataJ(u);
	write(nsd,&u,sizeof(u));
}

admin readDataA(admin a){

	int fd = open("Admin.txt",O_RDONLY);

	struct flock lock;				//initialising the lock.
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (a.userId)*sizeof(admin);	//lock starts from the starting of the required record.
	//lock.l_start = recordNum;	
	lock.l_len = sizeof(admin);
	lock.l_pid = getpid();

	
	int k=fcntl(fd,F_SETLKW,&lock);		

	lseek(fd,a.userId*(sizeof(admin)),SEEK_SET);
	read(fd,&a,sizeof(a));	
	
	lock.l_type = F_UNLCK;
	int j=fcntl(fd,F_SETLK,&lock);	
	close(fd);
	return a;
}

jointUser readDataJ(jointUser a){

	int fd = open("Joint.txt",O_RDONLY);

	struct flock lock;				//initialising the lock.
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (a.userId)*sizeof(jointUser);	//lock starts from the starting of the required record.
	//lock.l_start = recordNum;	
	lock.l_len = sizeof(jointUser);
	lock.l_pid = getpid();

	int k=fcntl(fd,F_SETLKW,&lock);

	lseek(fd,a.userId*(sizeof(jointUser)),SEEK_SET);
	read(fd,&a,sizeof(a));	
	
	lock.l_type = F_UNLCK;
	int j=fcntl(fd,F_SETLK,&lock);

	close(fd);	
	return a;
}


normalUser readDataN(normalUser a){

	int fd = open("normalUsers.txt",O_RDONLY);
	struct flock lock;				//initialising the lock.
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (a.userId)*sizeof(normalUser);	//lock starts from the starting of the required record.
	//lock.l_start = recordNum;	
	lock.l_len = sizeof(normalUser);
	lock.l_pid = getpid();
	
	int k=fcntl(fd,F_SETLKW,&lock);	

	lseek(fd,a.userId*(sizeof(normalUser)),SEEK_SET);
	read(fd,&a,sizeof(a));
	
	lock.l_type = F_UNLCK;
	int j=fcntl(fd,F_SETLK,&lock);
	
	close(fd);	
	return a;
}

void updateDataN(normalUser a){

	int fd = open("normalUsers.txt",O_RDWR);

	struct flock lock;				//initialising the lock.
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (a.userId)*sizeof(normalUser);	//lock starts from the starting of the required record.
	//lock.l_start = recordNum;	
	lock.l_len = sizeof(normalUser);
	lock.l_pid = getpid();
	//fd = open(argv[1],O_RDWR);
	
	int k=fcntl(fd,F_SETLKW,&lock);

	lseek(fd,a.userId*(sizeof(normalUser)),SEEK_SET);
	write(fd,&a,sizeof(a));
	
	lock.l_type = F_UNLCK;
	int j=fcntl(fd,F_SETLK,&lock);

	close(fd);
}

void updateDataJ(jointUser a){
	
	int fd = open("Joint.txt",O_RDWR);
	
	struct flock lock;				//initialising the lock.
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (a.userId)*sizeof(jointUser);	//lock starts from the starting of the required record.
	//lock.l_start = recordNum;	
	lock.l_len = sizeof(jointUser);
	lock.l_pid = getpid();
	//fd = open(argv[1],O_RDWR);
	
	int k=fcntl(fd,F_SETLKW,&lock);
	
	lseek(fd,a.userId*(sizeof(jointUser)),SEEK_SET);
	write(fd,&a,sizeof(a));

	lock.l_type = F_UNLCK;
	int j=fcntl(fd,F_SETLK,&lock);

	close(fd);
}

void updateDataA(admin a){
	
	int fd = open("Admin.txt",O_RDWR);
		
	struct flock lock;				//initialising the lock.
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (a.userId)*sizeof(admin);	//lock starts from the starting of the required record.
	//lock.l_start = recordNum;	
	lock.l_len = sizeof(admin);
	lock.l_pid = getpid();
	//fd = open(argv[1],O_RDWR);
	
	int k=fcntl(fd,F_SETLKW,&lock);
		
	lseek(fd,a.userId*(sizeof(admin)),SEEK_SET);
	write(fd,&a,sizeof(a));

	lock.l_type = F_UNLCK;
	int j=fcntl(fd,F_SETLK,&lock);	

	close(fd);
}

int main(){
	
	int clilen,sd,nsd;	
	char buff[25];
	struct sockaddr_in serv,cli;
	sd = socket(AF_INET,SOCK_STREAM,0);
		
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(5555);
	bind(sd,(struct sockaddr*)&serv,sizeof(serv));
	
	listen(sd,5);
	write(1,"Waiting for clients\n",sizeof("Waiting for clients\n"));	

	while(1){
		

		clilen = sizeof(cli);	
		nsd=accept(sd,(struct sockaddr *)&cli,&clilen);
		
		if(!fork()){
	
			close(sd);
			
			int type,len,valid =0;
		
			write(1,"Connected To Clients\n",sizeof("Connected To Clients\n"));
		
			admin a;jointUser a1;normalUser a2;		

			while(valid==0){

				read(nsd,&type,sizeof(type));		//Reading the type of the user
				//printf("Type : %d\n",type);
						

			if(type==1){
				write(1,"Admin\n",7);
				read(nsd,&a,sizeof(admin));
				valid=validateAdmin(a);
				//valid=1;				
				write(nsd,&valid,sizeof(valid));
				printf("username : %d\nPassword :%s\n",a.userId,a.password);
			}
			else if(type==2){
				write(1,"JointUsers\n",7);
				//jointUser a1;
				read(nsd,&a1,sizeof(jointUser));
				valid = validatejointUser(a1);				
				write(nsd,&valid,sizeof(valid));
				printf("username : %d\nPassword :%s\n",a1.userId,a1.password);
			}
			else if(type==3){
				write(1,"Normal User\n",7);
				read(nsd,&a2,sizeof(normalUser));
				valid=validatenUser(a2);
				write(nsd,&valid,sizeof(valid));
				printf("username : %d\nPassword :%s\n",a2.userId,a2.password);	
			}
		}
		
		int funcOption;		

		while(1){

			read(nsd,&funcOption,sizeof(funcOption));
			int retVal=0;
			
			if(type==3){

				//a2=readDataN(a2);

				if(funcOption==1){
					if(DepositN(nsd,a2))retVal=1;
					write(nsd,&retVal,sizeof(retVal));
				}
				else if(funcOption==2){
					if(WithdrawN(nsd,a2))retVal=1;
					write(nsd,&retVal,sizeof(retVal));
				}
				else if(funcOption==3){
					BalanceEnquiryN(nsd,a2);
				}
				else if(funcOption==4){
					if(PasswordChangeN(nsd,a2))retVal=1;
					write(nsd,&retVal,sizeof(retVal));
				}
				else if(funcOption==5){
					viewDetailsN(nsd,a2);
				}
				else if(funcOption==6){
					break;
				}
			}

			else if(type==1){
				
				//a = readDataA(a);
			
				int accType;

				if(funcOption==1){
					read(nsd,&accType,sizeof(accType));
					if(addAccount(nsd,accType))retVal=1;					
					write(nsd,&retVal,sizeof(retVal));
				}
				else if(funcOption==2){
					read(nsd,&accType,sizeof(accType));
					if(deleteAccount(nsd,accType))retVal=1;
					write(nsd,&retVal,sizeof(retVal));				
				}
				else if(funcOption==3){
					read(nsd,&accType,sizeof(accType));
					if(modifyAccount(nsd,accType))retVal=1;
					write(nsd,&retVal,sizeof(retVal));
				}
				else if(funcOption==4){
					read(nsd,&accType,sizeof(accType));
					if(searchAccount(nsd,accType))retVal=1;
					//write(nsd,&retVal,sizeof(retVal));			
				}

				else if(funcOption==5){
					break;
				}
			}
			
			else if(type==2){
				
				
				if(funcOption==1){
					if(DepositJ(nsd,a1))retVal=1;
					write(nsd,&retVal,sizeof(retVal));
				}
				else if(funcOption==2){
					if(WithdrawJ(nsd,a1))retVal=1;
					write(nsd,&retVal,sizeof(retVal));
				}
				else if(funcOption==3){
					BalanceEnquiryJ(nsd,a1);
				}
				else if(funcOption==4){
					if(PasswordChangeJ(nsd,a1))retVal=1;
					write(nsd,&retVal,sizeof(retVal));
				}
				else if(funcOption==5){
					viewDetailsJ(nsd,a1);
				}
				
				else if(funcOption==6){
					break;
				}	
		
			}

		}
		
		close(nsd);
		exit(0);

		}

	else close(nsd);
	}
}
