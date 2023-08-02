/**
 * @file addbook.h
 * @author Sohaib Mohamed (sohaib.amhmd@gmail.com)
 * @brief Header file to connect db and apply basic tbs funcs.
 * @version 0.1
 * @date 2023-08-02
 * @page this file is addbook.h
 *
 * @copyright Copyright (c) 2023
 * Released under the GNU GPLv2+,
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 *
 */
#ifndef HEADER_addbook
#define HEADER_addbook

#include <sqlite3.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Record Object. each element represent one DB column
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 */
typedef struct Record {
   int id;                // Recodrd id
   char id_buff[100];     // used for Convert & print
   char f_name[56];       // first name
   char l_name[56];       // last name
   char phone[24];        // Phone number
   char email[102];       // email address
   int street;            // street_num
   char street_buff[100]; // used for Convert & print
   char town[50];         // town
   char country[50];      // country
} Record;


/**
 * @brief Print welcome ascii message
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 */
void adbk_welcome_mgs(void);
/**
 * @brief Print detailed help list
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 */
void adbk_help_mgs(void);
/**
 * @brief Print one line options
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 */
void adbk_print_options_menu(void);
/**
 * @brief Connect to the Database
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 * @param db_name database name
 * @return sqlite3* sqlite obj return value
 */
sqlite3* adbk_db_connect(char* db_name);
/**
 * @brief Close the database connection
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 * @param db databse name
 */
void adbk_db_disconnect(sqlite3* db);
/**
 * @brief Create new table Called Record
 * @details This function is too tolerant. will not overwrite the table if not exist
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 * @param db database name
 * @return int FIXME void?
 */
int adbk_db_record_new(sqlite3* db);
/**
 * @brief Add new record to the Record Table
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 * @param db databse name
 * @param record Record object
 * @return int return 0 if successed
 */
int adbk_db_record_add(sqlite3* db, Record* record);
/**
 * @brief Remove record from database
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 * @param db database name
 * @param id the ID to delete it
 * @return int return 0 if successed
 */
int adbk_db_record_remove(sqlite3* db, int id);
/**
 * @brief list all Record table records
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 * @param db
 * @return int
 */
int adbk_db_record_select_all(sqlite3* db);
/**
 * @brief Search by name or part of name
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 * @param db database handler
 * @param keyword the word to search by...
 * @return int return value!
 */
int adbk_db_record_search(sqlite3* db, char* keyword);
/**
 * @brief Modify a record
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 * @param db database handler
 * @param id the id you want to change
 * @return int the return value!
 */
int adbk_db_record_modify(sqlite3* db, int id);
/**
 * @brief tbs print routine acts like printf
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 * @param format
 * @param ...
 */
void adbk_print(const char* format, ...);
/**
 * @brief Print table contains just one row
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 * @param id
 * @param record
 */
void adbk_print_table_onerow(int id, Record* record);
/**
 * @brief Print complete table with many rows
 * @details This is just a draft comment. Writing a real document for the APIs
 * is an intensive task needs so much time. so I will skip that for now.
 *
 * @param total_rows integer number represt the total number of row
 * @param records records!
 */
void adbk_print_table(int total_rows, Record* records);

#endif
