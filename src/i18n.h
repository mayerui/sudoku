#pragma once

#include <cstdint>
#include <map>
#include <string>

enum class Language : uint32_t {
  ENGLISH,
  CHINESE,
  MAX,
};

class I18n {
 public:
  enum class Key {
    ASK_KEY_MAP,
    INPUT_ERROR,
    LOAD_PROGRESS_FAIL,
    ASK_QUIT,
    ASK_SAVE,
    ASK_SAVE_PATH,
    FILE_EXIST_ERROR,
    CONTINUE,
    UNDO_ERROR,
    CONGRATULATION,
    NOT_COMPLETED,
    ASK_DIFFICULTY,
    GENERATING_SUDOKU,
  };

  using Dict = std::map<Key, std::string>;

  //单例设计模式, 确保程序中只存在一个 I18n 类的实例
  static I18n& Instance();

  void SetLanguage(Language);
  std::string Get(Key) const;

 private:
  I18n();

  Dict* dict_;
};
