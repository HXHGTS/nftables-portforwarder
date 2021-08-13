#!/bin/sh

echo "正在写入系统配置文件. . ."

curl https://cdn.jsdelivr.net/gh/HXHGTS/TCPOptimization/limits.conf > /etc/security/limits.conf

curl https://cdn.jsdelivr.net/gh/HXHGTS/TCPOptimization/sysctl.conf > /etc/sysctl.conf

sysctl -p

echo "正在安装nftables与nslookup. . ."

yum install nftables bind-utils -y

echo "正在配置nftables默认规则. . ."

nft flush table ip filter

nft delete chain ip filter

echo "正在启动nftables. . ."

systemctl restart nftables && systemctl enable nftables

exit
