#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "timetable.h"

#define MAXIMUM_TOKEN_SIZE

void makeTimeTableSpecification(char *buffer, uint size);
void deleteTimeTableSpecification(TimeTableSpecification *time_table_specification);

#endif
