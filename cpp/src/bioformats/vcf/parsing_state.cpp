#include "validator.hpp"

namespace opencb
{
  namespace vcf
  {

    ParsingState::ParsingState(
        std::shared_ptr<Source> source,
        std::shared_ptr<std::vector<Record>> records)
    : n_lines{1}, n_columns{1}, n_batches{0}, cs{0}, source{source}, records{records}
    {
    }

    void ParsingState::set_version(std::string & version) const
    {
        source->version = version;
    }
    
    void ParsingState::add_meta(MetaEntry const & meta) const
    {
        source->meta_entries.push_back(meta);
    }
    
    void ParsingState::add_record(Record const & record) const
    {
        records->push_back(record);
    }
    
    std::vector<std::string> const & ParsingState::samples() const
    {
        return source->samples_names;
    }
        
    void ParsingState::set_samples(std::vector<std::string> & samples) const
    {
        source->samples_names = samples;
    }
    
  }
}
