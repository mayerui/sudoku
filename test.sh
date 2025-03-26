#!/bin/bash

set -x  # 启用逐步调试

# 假设原始文件路径
SOURCE_FILE="string.xml"

# 目标语言代码列表（可以根据需求修改）
languages=("en" "fr" "es")  # 目标语言（英语、法语、西班牙语）

# 临时文件夹
TEMP_DIR="temp_translations"
mkdir -p "$TEMP_DIR"

# 1. 提取包含 '¥%' 符号的字段
pcregrep -o '<string name="([^"]+)">([^<]*¥%[^<]*)</string>' "$SOURCE_FILE" | while read line; do
  # 提取字段名和原始值
  name=$(echo $line | sed -E 's/<string name="([^"]+)">.*/\1/')
  value=$(echo $line | sed -E 's/<string name="[^"]+">([^<]+)<\/string>/\1/')
  
  # 输出找到的包含 '¥%' 的字段
  echo "Found: name=$name, value=$value"
  
  # 2. 使用 trans 命令翻译该字段
  for lang in "${languages[@]}"; do
    translated_value=$(trans -b :$lang "$value" | tail -n 1)  # 翻译并获取翻译结果
    
    # 3. 确保目标语言文件夹存在
    mkdir -p "res/values-$lang"

    touch "res/values-$lang/strings.xml"
    
    # 4. 在目标语言文件中找到倒数第二行的位置并插入新字段
    if pcregrep -q "<string name=\"$name\">" "res/values-$lang/strings.xml"; then
      # 如果目标语言文件已经存在该字段，替换
      if [[ "$OSTYPE" == "darwin"* ]]; then
        sed -i "" "s|<string name=\"$name\">.*</string>|<string name=\"$name\">$translated_value</string>|" "res/values-$lang/strings.xml"
      else
        sed -i "s|<string name=\"$name\">.*</string>|<string name=\"$name\">$translated_value</string>|" "res/values-$lang/strings.xml"
      fi
    else
      # 如果目标语言文件没有该字段，找到倒数第二行插入
      total_lines=$(wc -l < "res/values-$lang/strings.xml")
      second_last_line=$((total_lines - 1))
      
      # 在倒数第二行前插入新字段
      if [[ "$OSTYPE" == "darwin"* ]]; then
# sed -i "" "1i\\
# <string name=\"$name\">$translated_value</string>" "res/values-$lang/strings.xml"

 sed -i "" "$((total_lines))i\\
<string name=\"$name\">$translated_value</string>" "res/values-$lang/strings.xml"
      else
sed -i "" "1i\\
<string name=\"$name\">$translated_value</string>" "res/values-$lang/strings.xml"


      fi
    fi
  done
done
