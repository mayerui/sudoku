#pragma once

#include <map>
#include <string>

enum class Language {
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
  };
  using Dict = std::map<Key, std::string>;

  static I18n& Instance();
  void SetLanguage(Language);
  std::string Get(Key) const;

 private:
  I18n();

  Dict* dict_;
};
