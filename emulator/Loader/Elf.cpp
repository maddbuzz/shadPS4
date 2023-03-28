#include "Elf.h"


static self_header* load_self(FsFile& f)
{
	//read self header
	auto* self = new self_header;
	f.Read(self, sizeof(self_header));
	return self;
}

void Elf::Open(const std::string& file_name)
{
	m_f = new FsFile;
	m_f->Open(file_name, fsOpenMode::fsRead);

	m_self = load_self(*m_f);
    if (!isSelfFile())
    {
        delete m_self;
        m_self = nullptr;
        m_f->Seek(0,fsSeekMode::fsSeekSet); //it is not an self file move to the start of file
    }


    DebugDump();
}

bool Elf::isSelfFile() const
{
    if (m_f == nullptr)
    {
        return false;
    }

    if (m_self == nullptr)
    {
        return false;//if we can't load self header return false
    }

    if (m_self->magic != self_header::signature)
    {
        printf("Not a SELF file. Magic file mismatched! current = 0x%08X required = 0x%08X\n", m_self->magic, self_header::signature);
        return false;
    }

    if (m_self->version != 0x00 || m_self->mode != 0x01 || m_self->endian != 0x01 || m_self->attributes != 0x12)
    {
        printf("Unknown SELF file\n");
        return false;
    }

    if (m_self->category != 0x01 || m_self->program_type != 0x01)
    {
        printf("Unknown SELF file\n");
        return false;
    }

    return true;
}

void Elf::DebugDump() {
    printf("SELF header:\n");
    printf("  magic ..............: 0x%08X\n", m_self->magic);
    printf("  version    .........: %d\n", m_self->version);
    printf("  mode       .........: 0x%X\n", m_self->mode);
    printf("  endian     .........: %d\n", m_self->endian);
    printf("  attributes .........: 0x%X\n", m_self->attributes);
    printf("  category   .........: 0x%X\n", m_self->category);
    printf("  program_type........: 0x%X\n", m_self->program_type);
    printf("  padding1 ...........: 0x%04X\n", m_self->padding1);
    printf("  header size ........: 0x%X\n", m_self->header_size);
    printf("  meta size      .....: 0x%X\n", m_self->meta_size);
    printf("  file size ..........: 0x%X\n", m_self->file_size);
    printf("  padding2 ...........: 0x%08X\n", m_self->padding2);
    printf("  segment count ......: %u\n", m_self->segment_count);
    printf("  unknown 1A .........: 0x%04X\n", m_self->unknown1A);
    printf("  padding3 ...........: 0x%04X\n", m_self->padding3);
    printf("\n");
}