import ctypes
import numpy as np
from numpy.ctypeslib import ndpointer

metronome_lib = ctypes.cdll.LoadLibrary("/home/abhirath/projects/Metronome/src/metronome.so")

m_solve_timetable = metronome_lib.solveTimeTablingProblem
m_solve_timetable.restype = None
m_solve_timetable.argtypes = [
    # For party table
    ctypes.c_size_t,
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),

    # For venue table
    ctypes.c_size_t,
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),

    # For session table
    ctypes.c_size_t,
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),   
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),

    # For assignments table
    ctypes.c_size_t,
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),   
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),
    ndpointer(ctypes.c_uint,
              flags = "C_CONTIGUOUS"),
]

def solve_timetable(party_table, venue_table, session_table, assignment_table):
    get_table_size = lambda x : len(x[list(x.keys())[0]])

    get_np_uint = lambda x : np.ascontiguousarray(np.array(x, dtype = np.uintc))

    # For party table
    p_size = get_table_size(party_table)
    p_party_id = get_np_uint(party_table['party_id']) 
    p_strength = get_np_uint(party_table['strength'])

    # For venue table
    v_size = get_table_size(venue_table)
    v_venue_id = get_np_uint(venue_table['venue_id'])
    v_venue_type = get_np_uint(venue_table['venue_type'])
    v_capacity = get_np_uint(venue_table['capacity'])
    v_number_available = get_np_uint(venue_table['number_available'])

    # For session table
    s_size = get_table_size(session_table)
    s_session_id = get_np_uint(session_table['session_id'])
    s_venue_type = get_np_uint(session_table['venue_type'])
    s_duration = get_np_uint(session_table['duration'])

    # For assignment table
    a_size = get_table_size(assignment_table)
    a_party_id = get_np_uint(assignment_table['party_id'])
    a_session_id = get_np_uint(assignment_table['session_id'])
    a_priority = get_np_uint(assignment_table['priority'])

    m_solve_timetable(
        p_size,
        p_party_id,
        p_strength,

        # For venue table
        v_size,
        v_venue_id,
        v_venue_type,
        v_capacity,
        v_number_available,

        # For session table
        s_size,
        s_session_id,
        s_venue_type,
        s_duration,

        # For assignments table
        a_size,
        a_party_id,
        a_session_id,
        a_priority
    )
