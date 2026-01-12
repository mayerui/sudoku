#pragma once

#include <cstdint>
#include <map>
#include <string>
/*
数独游戏国际化（Internationalization，简称 i18n） 的核心头文件，
它通过单例模式封装了多语言配置逻辑，用枚举标准化提示文字的类型，
用字典映射不同语言的文本，实现了游戏所有交互提示的多语言切换，是整个项目支持多语言的基础核心。
*/

// 语言类型枚举
enum class Language : uint32_t {
  ENGLISH, // 英语
  CHINESE, // 中文
  MAX,  // 语言类型最大值，用于边界检查
};

// 国际化核心类
class I18n {
  // 提示文字的key枚举，每个key对应一类交互提示，避免硬编码字符串
 public:
  enum class Key {
    ASK_KEY_MAP,          // 询问按键模式（如“请选择操作模式：1-普通 2-Vim”）
    INPUT_ERROR,          // 输入错误（如“输入错误，请重新选择！”）
    LOAD_PROGRESS_FAIL,   // 加载进度失败
    ASK_QUIT,             // 询问是否退出
    ASK_SAVE,             // 询问是否保存
    ASK_SAVE_PATH,        // 询问保存路径
    FILE_EXIST_ERROR,     // 文件已存在错误
    CONTINUE,             // 继续（如“按任意键继续”）
    UNDO_ERROR,           // 撤销错误（如“无操作可撤销”）
    CONGRATULATION,       // 恭喜（数独完成）
    NOT_COMPLETED,        // 未完成（数独校验失败）
    ASK_DIFFICULTY,       // 询问难度（如“请选择难度：1-简单 2-普通 3-困难”）
  };
  //类型别名：简化”Key-文字“字典的写法
  using Dict = std::map<Key, std::string>;

  // 单例模式核心：获取全局唯一的I18n实例
  static I18n& Instance();
  // 设置当前语言
  void SetLanguage(Language);
  // 根据key获取当前语言的提示文字
  std::string Get(Key) const;

 private:
 //私有构造函数：禁止外部创建实例，单例模式中的关键
  I18n();

  //指向当前语言字典的指针（切换语言是修改该指针）
  Dict* dict_;
};
