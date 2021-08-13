#include <stdlib.h>
#include <stdio.h>

int ServerStartNum, ServerEndNum, NATStartNum, NATEndNum, PortGap, ServerPort, NATPort, mode;
char cmd[300], protocol[5], local_ip[16], ip[40];
FILE* info, * resolve;

int main() {
	preload();
	system("clear");
MENU:UI();
	if (mode == 1) {
		CheckNAT();
		printf("������Զ�̷�������ʼ�˿ں�-��ֹ�˿ںţ���10000-20000:");
		scanf("%d-%d", &ServerStartNum, &ServerEndNum);
		printf("\n");
		printf("��������ת��������ʼ�˿ں�-��ֹ�˿ںţ���10000-20000:");
		scanf("%d-%d", &NATStartNum, &NATEndNum);
		printf("\n");
		printf("������˿ںż��:");
		scanf("%d", &PortGap);
		printf("\n");
		printf("������Զ�̷�����������ip����8.8.8.8:");
		scanf("%s", ip);
		printf("\n");
		printf("������ת��Э��(t=tcp or u=udp):");
		scanf("%s", protocol);
		printf("\n");
		if (CheckInput() == 1) {
			printf("�Ƿ����룬�������룡\n");
		}
		else {
			NATProtocol();
			AddNAT();
			printf("\n\n");
			printf("ִ�����!\n");
			goto MENU;
		}
	}
	else if (mode == 2) {
		DelNAT();
		printf("\n\n");
		printf("ִ����ɣ�\n");
		goto MENU;
	}
	else if (mode == 3) {
		CheckNAT();
		goto MENU;
	}
	else if (mode == 4) {
		printf("����ɾ��nftables����. . .\n");
		system("nft flush table ip filter");
		printf("���ڹر�nftablesģ��. . .\n");
		system("systemctl stop nftables");
		system("systemctl disable nftables");
		printf("ִ����ɣ�\n");
		goto MENU;
	}
	else if (mode == 5) {
		system("curl -sSL https://cdn.jsdelivr.net/gh/HXHGTS/NATConfigGenerator-iptables/install_nftables.sh | sh");
		printf("ִ����ɣ�\n");
		goto MENU;
	}
	else {
		exit(0);
	}
	return 0;
}

int Domain_Transfer() {
	if (ip[0] != '0' && ip[0] != '1' && ip[0] != '2' && ip[0] != '3' && ip[0] != '4' && ip[0] != '5' && ip[0] != '6' && ip[0] != '7' && ip[0] != '8' && ip[0] != '9') {
		sprintf(cmd, "nslookup -q=A %s | grep 'Address:' | grep -v '#53' > dns.resolve", ip);
		system(cmd);
		resolve = fopen("dns.resolve", "r");
		fscanf(resolve, "Address: %s", ip);
		fclose(resolve);
		system("rm -f dns.resolve");
	}
	return 0;
}

int UI() {
	printf("��ע�⣺�������֧��CentOS7/8ϵͳ������Linuxϵͳ��֧�֣�\n\n");
	printf("��ѡ��Ҫִ�еĲ�����\n\n1.���ת������\n\n2.ɾ��ת������\n\n3.��ѯ������ת������\n\n4.�ر�nftables\n\n5.��װ������nftables(��һ��ʹ�ý���ִ�У�������ܻ�ת��ʧ��)\n\n0.�˳�\n\n�����룺");
	scanf("%d", &mode);
	printf("\n");
	return 0;
}

int AddNAT() {
	Domain_Transfer();
	for (ServerPort = ServerStartNum, NATPort = NATStartNum; ServerPort <= ServerEndNum; ServerPort = ServerPort + PortGap, NATPort = NATPort + PortGap) {
		sprintf(cmd, "nft add rule ip filter FORWARD %s dport %d counter dnat to %s:%d", protocol, NATPort, ip, ServerPort);
		system(cmd);
	}
	return 0;
}

int DelNAT() {
	int rule;
	CheckNAT();
	printf("��վ����վ����ͬʱ���ã����ɾ���ᱻͬʱɾ����\n");
	printf("��Ҫɾ���ڼ��й���ֻ����վ����վ���򼴿ɣ�:");
	scanf("%d", &rule);
	sprintf(cmd, "nft delete rule ip filter FORWARD handle %d", rule);
	system(cmd);
	system("clear");
	CheckNAT();
	return 0;
}

int CheckNAT() {
	printf("ϵͳNAT���ü�¼:\n");
	system("nft list table ip nat");
	printf("\n");
	printf("���ڱ��ݹ���. . .\n");
	system("echo 'ϵͳNAT���ü�¼:' > route.backup");
	system("nft list table ip nat  >> route.backup");
	printf("�����ѱ�����/root/route.backup\n");
	return 0;
}

int CheckInput() {
	if (mode == 1) {
		if (ServerStartNum > ServerEndNum || NATStartNum > NATEndNum || ServerStartNum > 65535 || ServerEndNum > 65535 || NATStartNum > 65535 || NATEndNum > 65535) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

int NATProtocol() {
	if (protocol[0] == 't') {
		sprintf(protocol, "tcp");
	}
	else {
		sprintf(protocol, "udp");
	}
	return 0;
}

int preload() {
	char netmask[16], public_ip[16];
	printf("���ڻ�ȡ������Ϣ. . .\n");
	system("ifconfig | grep inet | grep -v inet6 | grep -v 127.0.0.1 > net_info.txt");
	info = fopen("net_info.txt", "r");
	fscanf(info, "        inet %s  netmask %s  broadcast %s", local_ip, netmask, public_ip);
	fclose(info);
	system("rm -f net_info.txt");
}