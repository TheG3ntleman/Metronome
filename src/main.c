#include "common/file.h"
#include "common/string.h"
#include "timetable.h"
#include "parser.h"

int main(int argc, char **argv) {

  // TODO: Argument parsing
  if (argc > 1) {
    File *file = fileOpen(argv[1]);
    //printf("file contents: %s\n", file->str);

    // Testing parsing
    TimeTableSpecification * tts = ttsFromBuffer(file->str, file->size); 
    ttsPrintSpecifications(tts);

    /*
     * 1. Genereate a valid random sample population
     * 2. Estimate fitness of all organisms [CAN RUN ON GPU]
     * 3. Rank the organisms
     * 4. Generate offspring by cross-breeding scheme []
     * 5. mutate some offspring (optional) [CAN RUN ON GPU]
     * 3-5'. Some other technique to generate a new population.
     * 6. repeat from step 2 until stopping condition
     * */

     
    ttsDelete(tts);
    fileClose(file);
  }
  
  // Testing tts
  /*TimeTableSpecification *tts = ttsMake();
  ttsAddParty(tts, "CLASS_A");
  ttsAddParty(tts, "CLASS_B");
  ttsAddParty(tts, "CLASS_C");
  ttsAddParty(tts, "CLASS_D");
  ttsAddJob(tts, "Physics-II");
  ttsAddJobRequirement(tts, "Physics-II", "CLASS_C");
  ttsAddJobRepititions(tts, "Physics-II", 5);
  ttsAddJobDuration(tts, "Physics-II", 30);
  ttsAddVenue(tts, "ELT_2");
  ttsAddVenueCapacity(tts, "ELT_2", 300);
  ttsPrintSpecifications(tts);
  ttsDelete(tts);*/

  return 0;
}
