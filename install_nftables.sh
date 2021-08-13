#!/bin/sh

echo "正在写入系统配置文件. . ."

curl https://cdn.jsdelivr.net/gh/HXHGTS/TCPOptimization/limits.conf > /etc/security/limits.conf

curl https://cdn.jsdelivr.net/gh/HXHGTS/TCPOptimization/sysctl.conf > /etc/sysctl.conf

sysctl -p

echo "正在安装nftables与nslookup. . ."

systemctl stop firewalld && systemctl disable firewalld

systemctl stop iptables && systemctl disable iptables

setenforce 0

yum install nftables bind-utils -y

echo "正在配置nftables默认规则. . ."

nft add table ip filter

nft add chain ip filter INPUT

nft add chain ip filter FORWARD

nft add chain ip filter OUTPUT

add rule ip filter FORWARD tcp dport 22 ct state new counter accept

add rule ip filter FORWARD tcp dport 80 ct state new counter accept

echo "正在启动nftables. . ."

systemctl restart nftables && systemctl enable nftables

exit
