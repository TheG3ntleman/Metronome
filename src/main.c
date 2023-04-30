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
  ttsAddParty(tts, "CLASS_A");
  ttsAddParty(tts, "CLASS_B");
  ttsAddParty(tts, "CLASS_C");
  ttsAddParty(tts, "CLASS_D");
  ttsAddJob(tts, "Physics-II");
  ttsAddJobRequirement(tts, "Physics-II", "CLASS_C");
  ttsAddJobRepititions(tts, "Physics-II", 5);
  ttsAddJobDuration(tts, "Physics-II", 30);
  ttsPrintSpecifications(tts);
  ttsDelete(tts);

  return 0;
}
