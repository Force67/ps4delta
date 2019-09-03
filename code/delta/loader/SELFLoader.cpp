
// Copyright (C) Force67 2019

#include <cstdio>
#include <loader/SELFLoader.h>
#include <kernel/VMA.h>	
#include <utilities/File.h>

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7FFFFFFF
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xFFFFFFFF

enum ElfType {
	ET_NONE = 0,
	ET_REL = 1,
	ET_EXEC = 2,
	ET_DYN = 3,
	ET_CORE = 4,
	ET_LOPROC = 0xFF00,
	ET_HIPROC = 0xFFFF,
};

namespace loaders
{
#if 0
	SELF_Loader::SELF_Loader(std::unique_ptr<utl::File> file) :
		AppLoader(std::move(file))
	{}

	FileType SELF_Loader::IdentifyType(utl::File& file)
	{
		SELFHeader self{};
		file.Read(self);

		if (self.magic == SELF_MAGIC && self.contentType == SELFContentType::SELF) {
			std::puts("[+] Identified file as Signed ELF (SELF)");
			return FileType::SELF;
		}

		return FileType::UNKNOWN;
	}

	LoadErrorCode SELF_Loader::Load(krnl::VMAccessMgr& vma)
	{
		// reset fp
		file->Seek(0, utl::seekMode::seek_set);

		// read the initial header
		SELFHeader self{};
		file->Read(self);
	
		// load all SCE segments
		std::vector<SELFSegmentTable> scesegments(self.numSegments);
		file->Read(scesegments);

		// dump some info
		for (auto& e : scesegments) {
			std::printf("SceSeg >  Size: %llu | Offset: 0x%llx | SegID: %d\n",
				e.fileSize, e.offset, e.Id());
		}

		// find & verify elf header
		file->Read(elf);
		
		/*if (*(uint32_t*)& elf != 0x464C457F) {
			return LoadErrorCode::BADMAGIC;
		}*/

		// 0x40 relative to elf
		file->Seek(elf.phoff, utl::seekMode::seek_cur);
		segments.resize(elf.phnum);
		file->Read(segments);

		/*// dont even ask
		size_t offset = sizeof(ELFHeader) + (elf->phnum * sizeof(ELFPgHeader));
		offset += 15; offset &= ~15; // align

		SCESpecial* special = GetElfOfs<SCESpecial>(offset + 7);
		std::printf("SCE special located at offset %llx, %x\n", ((uintptr_t)special - (uintptr_t)data), (uint8_t)special->authId);*/

		if (MapSegments(vma))
			return LoadErrorCode::SUCCESS;

		return LoadErrorCode::UNKNOWN;
	}

	bool SELF_Loader::MapSegments(krnl::VMAccessMgr& vma)
	{
		// count total segment size we are interested in
		uint32_t total_size = 0;
		for (auto& seg : segments) {
			if (seg.type == PT_LOAD || seg.type == PT_SCE_RELRO) {
				total_size += (seg.memsz + 0xFFF) & ~0xFFF;
			}
		}
		
#if 0
		// reserve kernel memory
		void* mem = vma.AllocateSeg(total_size);
		std::printf("[+] Allocating  %s @%p <%d bytes>\n", TypeToString(), mem, total_size);

		// process segments
		uint32_t offset_hack = 0;
		for (uint16_t i = 0; i < elf->phnum; ++i) {

			const auto* p = &segments[i];

			uint32_t hack = offset_hack;
			offset_hack += p->filesz;

			// ignore info segments
			//if (p->filesz == 0)
			//	continue;

			//if (p->type == PT_LOAD) {
				std::printf("ELF SEG:%d, %llu (%llx) ELF TYPE %s, VIRT %p, PHYS %p, FILESIZE %lld, VSIZE %lld, HEADER OFS %p, OFFSET HACK %x\n",
					(int)i, (uint32_t)p->offset - p->align, (uint32_t)p->offset - p->align,
					SecTypeToStr(p->type),
					(void*)p->vaddr,
					(void*)p->paddr,
					p->filesz,
					p->memsz,
					(void*)((uintptr_t)p - (uintptr_t)data), hack);
				//FormatFlag(p->flags);
			//}

			switch (p->type) {
			case PT_DYNAMIC: {
				SetupDynamics(p);
			} break;
			case PT_TLS: {

				// if the application uses TLS...
				// TODO: check redudant..
				if (p->filesz > 0 && p->offset != 0)
					SetupTLS(p);
			} break;

			}
		}
#endif

		return true;
	}

	void SELF_Loader::SetupDynamics(const ELFPgHeader* pg)
	{
		/*auto* dynamics = GetOffset<ELFDyn>(pg->offset);
		for (uint32_t i = 0; i < pg->filesz / sizeof(ELFDyn); i++) {

			const auto* p = &dynamics[i];
			std::printf("%d dyamics found, dynamic type %x\n", i, p->tag);
			
		}*/
	}

	void SELF_Loader::SetupTLS(const ELFPgHeader* pg)
	{
		//TODO: allocate TLS memory
	}

	LoadErrorCode SELF_Loader::Unload() {
		return LoadErrorCode::UNKNOWN;
	}
#endif
}