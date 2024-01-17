#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timetable/specifications.h"
#include "metronome.h"

typedef unsigned int uint;

int main() {

	srand(time(NULL));

    // Sample data for demonstration purposes
    size_t p_size = 4;
    uint p_party_id[] = {1, 2, 3, 4}; // 1 -> party1, 2 -> party2, 3 -> teacher1, 4 -> teacher2
    uint p_strength[] = {50, 30, 1, 1};

    size_t v_size = 3;
    uint v_venue_id[] = {101, 102, 103}; // 101 -> venue1, 102 -> venue2, 103 -> venue3
    uint v_venue_type[] = {5, 5, 6}; // 5 -> lecture room, 6 -> lab
    uint v_capacity[] = {50, 50, 50};
    uint v_number_available[] = {1, 1, 1};

    size_t s_size = 7;
    uint s_session_id[] = {201, 202, 203, 204, 205, 206, 207}; // 201 -> session1, 202 -> session2, 203 -> session3
    uint s_venue_type[] = {5, 5, 6, 6, 5, 5, 5};
    uint s_duration[] = {1, 1, 2, 2, 1, 1, 1};

    size_t a_size = 6;
    uint a_party_id[] = {1, 1, 2, 3, 4, 4}; // teacher2 has a lecture session with class1 and teacher1 has a lecture session with class1
    uint a_session_id[] = {201, 203, 202, 202, 201, 203};
    uint a_priority[] = {1, 1, 1, 1, 1, 1};

    // Call the function with the prepared arguments
    solveTimeTablingProblem(
        p_size, p_party_id, p_strength,
        v_size, v_venue_id, v_venue_type, v_capacity, v_number_available,
        s_size, s_session_id, s_venue_type, s_duration,
        a_size, a_party_id, a_session_id, a_priority
    );

	return 0;
}
