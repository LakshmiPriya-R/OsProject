#include<unistd.h>
#include <stdio.h>
#include<sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<stdbool.h>
#include<string.h>
int userId;

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

int normalLogin(int sfd);
int jointLogin(int sfd);
int adminLogin(int sfd);
void giveNUserOptions(int);
void giveAdminOptions(int);
void giveJointOptions(int);
void Deposit(int);
void Withdraw(int);
void BalanceEnquiry(int);
void PasswordChange(int);
void ViewDetails(int,int);
void addAccount(int);
void deleteAccount(int);
void modifyAccount(int);
void searchAccount(int);

void Login(int sfd){

	int len,option=0;
	bool valid;
	write(1,"Login :\n",9);
	write(1,"1 : AdminLogin\n2 : JointUsersLogin\n3 : normalUserLogin\nEnter The Option : ",sizeof("1 : AdminLogin\n2 : JointUsersLogin\n3 : normalUserLogin\nEnter The Option : "));
	scanf("%d",&option);
	printf("Option : %d\n",option);
	//option -= 2608;
	int type;
	if(option==1){
		type=1;
		write(sfd,&type,sizeof(type));
		if(adminLogin(sfd))giveAdminOptions(sfd);
		else{
			printf("Wrong Details\n");
			Login(sfd);
		}
	}
	else if(option==2){
		type=2;
		write(sfd,&type,sizeof(type));
		if(jointLogin(sfd))giveJointOptions(sfd);
		else{
			printf("Wrong Details\n");
			Login(sfd);
		}
	}
	else if(option==3){
		type=3;
		write(sfd,&type,sizeof(type));
		if(normalLogin(sfd))giveNUserOptions(sfd);
		else{
			printf("Wrong Details\n");
			Login(sfd);
		}
	}
	else{
		write(1,"Enter a correct option\n",sizeof("Enter a correct option\n"));
		Login(sfd);
	}
}

void giveNUserOptions(int sfd){


	write(1,"1 : Deposit\n2: Withdraw\n3 : Balance Enquiry\n4 : Password Change\n5 : View details\n6 : Exit\n\nEnter Option : ",109);
	int option;
	scanf("%d",&option);
	int type;
	if(option==1){
		//normalUser nU;
		type = 1;
		write(sfd,&type,sizeof(type));
		Deposit(sfd);
		giveNUserOptions(sfd);
	}
	else if(option==2){
		type = 2;
		write(sfd,&type,sizeof(type));
		Withdraw(sfd);
		giveNUserOptions(sfd);
	}
	else if(option==3){
		type = 3;
		write(sfd,&type,sizeof(type));
		BalanceEnquiry(sfd);
		giveNUserOptions(sfd);
	}
	else if(option==4){
		type = 4;
		write(sfd,&type,sizeof(type));
		PasswordChange(sfd);
		giveNUserOptions(sfd);
	}
	else if(option==5){
		type = 5;
		write(sfd,&type,sizeof(type));
		ViewDetails(sfd,1);
		giveNUserOptions(sfd);
	}
	else if(option==6){
		type=6;write(sfd,&type,sizeof(type));
		exit(0);
	}
}

void giveAdminOptions(int sfd){

	write(1,"1 : Add Account\n2: Delete Account\n3 : Modify\n4 : Search Account\n5 : Exit\n\nEnter Option : ",95);
	int option;
	scanf("%d",&option);
	int type;
	if(option==1){
		type = 1;
		write(sfd,&type,sizeof(type));
		addAccount(sfd);
		giveAdminOptions(sfd);
	}
	else if(option==2){
		type = 2;
		write(sfd,&type,sizeof(type));
		deleteAccount(sfd);
		giveAdminOptions(sfd);
	}
	else if(option==3){
		type = 3;
		write(sfd,&type,sizeof(type));
		modifyAccount(sfd);
		giveAdminOptions(sfd);
	}
	else if(option==4){
		type = 4;
		write(sfd,&type,sizeof(type));
		searchAccount(sfd);
		giveAdminOptions(sfd);
	}
	else if(option==5){
		type=5;
		write(sfd,&type,sizeof(type));
		exit(0);
	}

}

void addAccount(int sfd){

	write(1,"1 : Add Normal Account\n2: Add Joint Account\n\nEnter Option : ",sizeof("1 : Add JointAccount\n2: Add Normal Account\n\nEnter Option : "));
	int option;
	scanf("%d",&option);
	write(sfd,&option,sizeof(option));

	if(option==1){
		normalUser nU;
		printf("Username : ");
		scanf(" %[^\n]",nU.username);
		printf("Password : ");
		scanf(" %[^\n]",nU.password);
		write(sfd,&nU,sizeof(jointUser));
	}
	else if(option==2){
		jointUser nU;
		printf("Username 1 : ");
		scanf(" %[^\n]",nU.username1);printf("Username 2 :");
		scanf(" %[^\n]",nU.username2);
		printf("Password : ");
		scanf(" %[^\n]",nU.password);
		write(sfd,&nU,sizeof(normalUser));
	}

	int result;

	read(sfd,&result,sizeof(result));
	if(result)printf("Account Added Successfully\n");
	else printf("Some Error Occurred At Server\n");
}

void deleteAccount(int sfd){

	int type;
	int userId;

	write(1,"Account Type : (INT)",sizeof("Account Type : (INT)"));
	scanf("%d",&type);

	write(sfd,&type,sizeof(type));

	write(1,"UserId : ",sizeof("UserId : "));
	scanf("%d",&userId);

	write(sfd,&userId,sizeof(userId));

	int result;
	read(sfd,&result,sizeof(result));
	
	if(result)write(1,"Account Deleted\n",sizeof("Account Deleted\n"));
	else write(1,"Error Occurred\n",sizeof("Error Occurred\n"));

}

void searchAccount(int sfd){

	int type,userId;

	write(1,"Account Type To Search : ",sizeof("Account Type To Search : "));
	scanf("%d",&type);

	write(sfd,&type,sizeof(int));

	write(1,"UserId :",sizeof("UserId :"));

	scanf("%d",&userId);

	write(sfd,&userId,sizeof(userId));

	if(type==1){

		normalUser u;
		read(sfd,&u,sizeof(normalUser));

		printf("UserId : %d\n",u.userId);
		printf("Username : %s\n",u.username);
		printf("Account Number : %ld\n",u.accountNumber);
		printf("Balance : %lf\n",u.balance);
		printf("Status : %d\n",u.status);
	}

	else if(type==2){

		jointUser nU;

		read(sfd,&nU,sizeof(jointUser));

		printf("UserId : %d\n",nU.userId);
		printf("Username1 : %s\n",nU.username1);
		printf("Username2 : %s\n",nU.username2);
		printf("Account Number : %ld\n",nU.accountNumber);
		printf("Balance : %lf\n",nU.balance);
		printf("Status : %d\n",nU.status);
	}

}
void modifyAccount(int sfd){

	int type;
	int userId;

	write(1,"Account Type : (INT)",sizeof("Account Type : (INT)"));
	scanf("%d",&type);

	write(sfd,&type,sizeof(type));

	if(type==1){

		normalUser nU;

		write(1,"UserId : ",sizeof("UserId : "));
		scanf("%d",&nU.userId);

		write(1,"UserName : ",sizeof("UserName : "));
		scanf(" %[^\n]",nU.username);

		write(1,"Password : ",sizeof("Password : "));
		scanf(" %[^\n]",nU.password);

		write(1,"Balance : ",sizeof("Balance : "));
		scanf("%lf",&nU.balance);

		write(1,"Status : ",sizeof("Status : "));
		scanf("%d",&nU.status);
		write(sfd,&nU,sizeof(nU));
	}


	else if(type==2){

		jointUser nU;

		write(1,"UserId : ",sizeof("UserId : "));
		scanf("%d",&nU.userId);

		write(1,"UserName 1 : ",sizeof("UserName 1 : "));
		scanf(" %[^\n]",nU.username1);

		write(1,"UserName 2 : ",sizeof("UserName 2 : "));
		scanf(" %[^\n]",nU.username2);

		write(1,"Password : ",sizeof("Password : "));
		scanf(" %[^\n]",nU.password);

		write(1,"Balance : ",sizeof("Balance : "));
		scanf("%lf",&nU.balance);

		write(1,"Status : ",sizeof("Status : "));
		scanf("%d",&nU.status);
		write(sfd,&nU,sizeof(nU));
	}

	int result;
	read(sfd,&result,sizeof(result));

	if(result)write(1,"Account Modified\n",sizeof("Account Modified\n"));
	else write(1,"Error Occurred\n",sizeof("Error Occurred\n"));

}

void giveJointOptions(int sfd){


	write(1,"1 : Deposit\n2: Withdraw\n3 : Balance Enquiry\n4 : Password Change\n5 : View details\n6 : Exit\n\nEnter Option : ",109);
	int option;
	scanf("%d",&option);

	int type;
	if(option==1){
		type = 1;
		write(sfd,&type,sizeof(type));
		Deposit(sfd);
		giveJointOptions(sfd);
	}
	else if(option==2){
		type = 2;
		write(sfd,&type,sizeof(type));
		Withdraw(sfd);
		giveJointOptions(sfd);
	}
	else if(option==3){
		type = 3;
		write(sfd,&type,sizeof(type));
		BalanceEnquiry(sfd);
		giveJointOptions(sfd);
	}
	else if(option==4){
		type = 4;
		write(sfd,&type,sizeof(type));
		PasswordChange(sfd);
		giveJointOptions(sfd);
	}
	else if(option==5){
		type = 5;
		write(sfd,&type,sizeof(type));
		ViewDetails(sfd,2);
		giveJointOptions(sfd);
	}
	else if(option==6){
		type=6;
		write(sfd,&type,sizeof(type));
		exit(0);
	}
}

void Deposit(int sfd){

	int amount,retVal;
	printf("Enter Amount to Deposit : ");
	scanf("%d",&amount);
	write(sfd,&amount,sizeof(amount));

	read(sfd,&retVal,sizeof(retVal));

	if(retVal)printf("Deposit Success\n");
	else printf("Deposit Failed\n");
}

void Withdraw(int sfd){

	int amount,retVal;
	printf("Enter Amount to Withdraw : ");
	scanf("%d",&amount);
	write(sfd,&amount,sizeof(amount));

	read(sfd,&retVal,sizeof(retVal));

	if(retVal)printf("Withdraw Success\n");
	else printf("Withdraw Failed\n");
}

void BalanceEnquiry(int sfd){

	int balance,retVal;

	read(sfd,&balance,sizeof(balance));

	printf("Balance : %d\n",balance);
}

void PasswordChange(int sfd){

	int retVal;char pass[30];

	printf("Enter New Password : ");
	scanf(" %[^\n]",pass);

	write(sfd,pass,sizeof(pass));

	read(sfd,&retVal,sizeof(retVal));

	if(retVal)printf("PasswordChange Success\n");
	else printf("PasswordChange Failed\n");
}

void ViewDetails(int sfd,int type){

	int retVal;

	if(type==1){

		normalUser u;
		read(sfd,&u,sizeof(normalUser));

		printf("UserId : %d\n",u.userId);
		printf("Username : %s\n",u.username);
		printf("Account Number : %ld\n",u.accountNumber);
		printf("Balance : %lf\n",u.balance);
	}

	else if(type==2){

		jointUser u;
		read(sfd,&u,sizeof(jointUser));

		printf("UserId : %d\n",u.userId);
		printf("Username1 : %s\n",u.username1);
		printf("Username2 : %s\n",u.username2);
		printf("Account Number : %ld\n",u.accountNumber);
		printf("Balance : %lf\n",u.balance);
	}

}


int adminLogin(int sfd){

	admin a;
	int valid=0;

	printf("UserId : ");
	scanf("%d",&a.userId);
	printf("Password : ");
	scanf(" %[^\n]",a.password);
	userId = a.userId;
	write(sfd,&a,sizeof(admin));

    	read(sfd,&valid,sizeof(valid));
    	if(valid==0)return 0;
    	else return 1;
}


int jointLogin(int sfd){

	jointUser a;
	int valid=0;

	printf("UserId : ");
	scanf("%d",&a.userId);
	printf("Password : ");
	scanf(" %[^\n]",a.password);
	userId = a.userId;
	write(sfd,&a,sizeof(jointUser));

    	read(sfd,&valid,sizeof(valid));
    	if(valid==0)return 0;
    	else return 1;
}

int normalLogin(int sfd){

	normalUser a;
	int valid=0;

	printf("UserId : ");
	scanf("%d",&a.userId);
	printf("Password : ");
	scanf(" %[^\n]",a.password);
	userId = a.userId;
	write(sfd,&a,sizeof(normalUser));

    	read(sfd,&valid,sizeof(valid));
    	if(valid==0)return 0;
    	else return 1;

}


int main(){

	int sd,len;
	struct sockaddr_in serv;
	sd = socket(AF_INET,SOCK_STREAM,0);

	serv.sin_family = AF_INET;
	//serv.sin_addr.s_addr=inet_addr("192.168.170.137");

	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv.sin_port = htons(5555);

	connect(sd,(struct sockaddr *)&serv,sizeof(serv));

	Login(sd);
}
