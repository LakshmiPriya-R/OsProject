#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

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


int main(int argc,char * argv[]){

	int choice=1;
	int fd=open("Admin.txt",O_CREAT|O_RDWR,0744);	
	admin a;
	a.userId=0;
	printf("Enter name : ");
	scanf(" %[^\n]",a.username);
	printf("Enter password : ");
	scanf(" %[^\n]",a.password);
	printf("Enter status : ");	
	scanf("%d",&a.status);
	
	write(fd,&a,sizeof(admin));

	close(fd);	
	
	fd = open("normalUsers.txt",O_CREAT|O_RDWR,0744);
	
	printf("Normal Users\n");

	normalUser nU;
	int userId = 0;	
	long int accNo = 1000001;
	while(choice){
		printf("Enter name : ");
		scanf(" %[^\n]",nU.username);
		printf("Enter password : ");
		scanf(" %[^\n]",nU.password);
		printf("Enter Balance : ");
		scanf("%lf",&nU.balance);
		printf("Enter status : ");
		scanf("%d",&nU.status);
		nU.userId = userId;
		nU.accountNumber = accNo;
		write(fd,&nU,sizeof(normalUser));
		printf("Do you want to continue (0/1): ");
		scanf("%d",&choice);
		userId++;
		accNo++;	
	}

	close(fd);	

	choice = 1;
	jointUser jU;
	fd = open("Joint.txt",O_CREAT|O_RDWR,0744);
	userId = 0;	
	accNo = 1000000;

	printf("JointUser\n");

	while(choice){
		printf("Enter name 1 : ");
		scanf(" %[^\n]",jU.username1);
		printf("Enter name 2 : ");
		scanf(" %[^\n]",jU.username2);
		printf("Enter password : ");
		scanf(" %[^\n]",jU.password);
		printf("Enter Balance : ");
		scanf("%lf",&jU.balance);
		printf("Enter status : ");
		scanf("%d",&jU.status);
		jU.userId = userId;
		jU.accountNumber = accNo;
		write(fd,&jU,sizeof(normalUser));
		printf("Do you want to continue (0/1): ");
		scanf("%d",&choice);
		userId++;
		accNo++;	
	}
	

	close(fd);
}

