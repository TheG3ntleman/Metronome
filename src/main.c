#include "common/file.h"
#include "common/string.h"
#include "timetable.h"

int main(int argc, char **argv) {

  // TODO: Argument parsing
  if (argc > 1) {
    File *file = fileOpen(argv[1]);
    printf("file contents: %s\n", file->str);
    fileClose(file);
  }
  
  // Testing tts
  TimeTableSpecification *tts = ttsMake();
  ttsAddParty(tts, "PHYSICS-II");
  ttsAddParty(tts, "ENGLISH");
  ttsAddParty(tts, "MECHANICS");
  ttsAddParty(tts, "REAL_ANALYSIS");
  ttsPrintSpecifications(tts);
  ttsDelete(tts);

  return 0;
}
