#!/bin/bash

# 测试脚本：调用 myschedule_cli 进行任务的增删查改

CLI="../build/src/cli/myschedule_cli"
USER="testuser"
PASS="123456"

echo "=== 1. 添加任务 ==="
$CLI $USER $PASS addtask "写大作业" "2026-07-30 14:00" high study "2026-07-30 13:50"
$CLI $USER $PASS addtask "健身" "2026-07-31 07:00" medium life "2026-07-31 06:50"
$CLI $USER $PASS addtask "买菜" "2026-07-31 18:00" low life ""

echo -e "\n=== 2. 显示所有任务 ==="
$CLI $USER $PASS showtask day 2026-07-30

echo -e "\n=== 3. 删除任务（假设ID为1） ==="
$CLI $USER $PASS deltask 1

echo -e "\n=== 4. 再次显示所有任务 ==="
$CLI $USER $PASS showtask day 2026-07-31

echo -e "\n=== 测试完成 ==="
