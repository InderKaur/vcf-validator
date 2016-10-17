/**
 * Copyright 2016 EMBL - European Bioinformatics Institute
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef VCF_SQLITE_REPORT_HPP
#define VCF_SQLITE_REPORT_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include "sqlite/sqlite3.h"
#include "vcf/error.hpp"
#include "report_writer.hpp"
#include "report_reader.hpp"

namespace ebi
{
  namespace vcf
  {
    class SqliteReportRW : public ReportWriter, public ReportReader
    {
      public:
        SqliteReportRW(std::string db_name);
        /** optional, the destructor handles the flushing and closing */
        void flush();
        /** optional, the destructor handles the flushing and closing */
        void close();
        ~SqliteReportRW() override;
        
        // ReportWriter implementation
        void write_error(Error &error) override;
        void write_warning(Error &error) override;
        
        // ReportReader implementation
        size_t count_errors() override;
        size_t count_warnings() override;
        
        void for_each_error(std::function<void(std::shared_ptr<Error>)> user_function) override;
        void for_each_warning(std::function<void(std::shared_ptr<Error>)> user_function) override;
        
      private:
        void write(const Error &error, sqlite3_stmt *statement);
        size_t count(std::string table);
        void for_each(std::string table, std::function<void(std::shared_ptr<Error>)> user_function);
        
        void start_transaction();
        void commit_transaction();
        void rollback_transaction();

        sqlite3* db;
        std::string db_name;
        size_t current_transaction_size;
        const size_t transaction_size;
        sqlite3_stmt *statement_error;
        sqlite3_stmt *statement_warning;
        const std::chrono::milliseconds sleep_time;
    };
  }
}


#endif // VCF_SQLITE_REPORT_HPP