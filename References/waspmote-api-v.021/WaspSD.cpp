/*
 *  Copyright (C) 2009 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
   
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
  
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version:		0.10
 *  Design:		David Gascón
 *  Implementation:	David Cuartielles, Alberto Bielsa, Roland Riegel, Ingo Korb, Aske Olsson
 */
 

#ifndef __WPROGRAM_H__
  #include "WaspClasses.h"
#endif

// Variables ///////////////////////////////////////////////////////////////////

// Constructors ////////////////////////////////////////////////////////////////

WaspSD::WaspSD()
{
    // nothing to do
}

// Public Methods //////////////////////////////////////////////////////////////

/*
 * ON (void) - It powers the SD card, initialize it and prepare it to work with
 *
 *  It powers the SD card, initialize it and prepare it to work with
 */
void WaspSD::ON(void)
{
	begin();
	setMode(SD_ON);
	init();
}


/*
 * OFF (void) - It powers off the SD card and closes the pointers
 *
 *  It powers off the SD card and closes the pointers
 */
void WaspSD::OFF(void)
{
	close();
	setMode(SD_OFF);
}


/* begin() - sets switch and sd_present pin as output and input
* 
*/
void	WaspSD::begin()
{
  // activate the power to the SD card
  pinMode(MEM_PW, OUTPUT);
  pinMode(SD_PRESENT, INPUT);
}


/* setMode() - sets energy mode
* 
* It sets SD ON/OFF, switching On or Off the coresponding pin
* 
*/
void	WaspSD::setMode(uint8_t mode)
{
	_pwrMode=mode;
	switch(_pwrMode)
	{
		case	SD_ON:
			digitalWrite(MEM_PW, HIGH);
			delay(10);
			break;
		case	SD_OFF:
			digitalWrite(MEM_PW,LOW);
			break;
	}
}


/* init ( void ) - initializes the use of SD cards, looks into the
 * root partition, opens the file system and initializes the
 * public pointer that can be used to access the filesystem
 * It answers a human readable string that can be printed
 * by the user directly.
 *
 * It updates the flagDOS with an error code indicating the
 * possible error messages:
 *
 * - NOTHING_FAILED 
 * - CARD_NOT_PRESENT
 * - INIT_FAILED 
 * - PARTITION_FAILED 
 * - FILESYSTEM_FAILED 
 * - ROOT_DIR_FAILED 
 */
char* WaspSD::init()
{

  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    return CARD_NOT_PRESENT_em;
  }
  

  // proceed with initialization
  if(!sd_raw_init())
  {
    flag = INIT_FAILED;
    return INIT_FAILED_em;
  }
  

  // open first partition
  partition = partition_open(sd_raw_read,
                            sd_raw_read_interval,
                            sd_raw_write,
                            sd_raw_write_interval,
                            0
                            );


  if(!partition)
  {
    // If the partition did not open, assume the storage device
    // is a "superfloppy", i.e. has no MBR.
    partition = partition_open(sd_raw_read,
                            sd_raw_read_interval,
                            sd_raw_write,
                            sd_raw_write_interval,
                            -1
                            );
    

    if(!partition)
    {
      flag = PARTITION_FAILED;
      return PARTITION_FAILED_em;
    }
  }
  

  // open file system 
  fs = fat_open(partition);
  if(!fs)
  {
    flag = FILESYSTEM_FAILED;
    return FILESYSTEM_FAILED_em;
  }
  

  // open root directory
  struct fat_dir_entry_struct directory;
  fat_get_dir_entry_of_path(fs, "/", &directory);


  dd = fat_open_dir(fs, &directory);
  if(!dd)
  {
    flag = ROOT_DIR_FAILED;
    return ROOT_DIR_FAILED_em;
  }
  

  flag = NOTHING_FAILED;
  return NOTHING_FAILED_em;
}


/*
 * close (void) - closes the directory, filesystem and partition pointers
 *
 * this function closes all the pointers in use in the library, so that they 
 * can be reused again after a call to init(). It becomes very useful if e.g.
 * the card is removed and inserted again
 */
void WaspSD::close()
{
  // close dir 
  fat_close_dir(dd);

  // close file system 
  fat_close(fs);

  // close partition 
  partition_close(partition);
  
  pinMode(SD_SS,INPUT);
  pinMode(SD_SCK,INPUT);
  pinMode(SD_MISO,INPUT);
  pinMode(SD_MOSI,INPUT);
}


/*
 * isSD (void) - returns 1 if SD card is present, 0 otherwise
 *
 * here we make a call to close(), to avoid errors if users tried to call
 * any functions making use of the card
 */
uint8_t WaspSD::isSD()
{
  if (digitalRead(SD_PRESENT))    return 1;
    
  // if the SD is not there, the best is to close all the pointers
  // just to avoid problems later one with calls to functions  
  close();
  return 0;
}


/*
 * getDiskSize (void) - disk total size
 *
 * answers the total size for the disk, but also updates the
 * DOS.diskSize variable.
 */
offset_t WaspSD::getDiskSize()
{
    struct fat_fs_struct* _fs;            // filesystem pointer
    _fs=fs;
    
    // check if the card is there or not
    if (!isSD())
    {
        flag = CARD_NOT_PRESENT;
        sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
        return 0;
    }
 
  // update the publicly available size-related variables
    diskSize = fat_get_fs_size(_fs);
    return diskSize;
}


/*
 * getDiskFree (void) - disk free space
 *
 * answers the total available space for the disk, but also updates the
 * DOS.diskFree variable.
 */
offset_t WaspSD::getDiskFree()
{
    struct fat_fs_struct* _fs;
    _fs = fs;
  
    // check if the card is there or not
    if (!isSD())
    {
        flag = CARD_NOT_PRESENT;
        sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
        return 0;
    }

  // update the publicly available size-related variables
    diskFree = fat_get_fs_free(_fs);
    return diskFree;
}

/*
 * print_disk_info (void) - disk information
 *
 * packs all the data about the disk into the buffer and returns it back. The
 * DS.buffer will then be available and offer the data to the developers as
 * a human-readable encoded string.
 *
 * An example of the output by this system would be:
 *
 * manuf:  0x2
 * oem:    TM
 * prod:   SD01G
 * rev:    32
 * serial: 0x9f70db88
 * date:   2/8
 * size:   947 MB
 * free:   991428608/992608256
 * copy:   0
 * wr.pr.: 0/0
 * format: 0
 *
 * Not all the information is relevant for further use. It is possible to access
 * different parts via the fat-implemented variables as you will see in this
 * function
 *
 * However, we have made a couple of the variables available for quick use. They are
 * the total avaiable space (DOS.diskFree) and the disk total size (DOS.diskSize). Both
 * are accounted in bytes
 */
char* WaspSD::print_disk_info()
{
    struct fat_fs_struct* _fs;
    _fs=fs;
    
    // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return buffer;
  }

    if(!fs)
        return 0;

    struct sd_raw_info disk_info;
    if(!sd_raw_get_info(&disk_info))
        return 0;

    // update the publicly available size-related variables
    diskFree = fat_get_fs_free(_fs);
    diskSize = fat_get_fs_size(_fs);
 
    sprintf(buffer, "" \
    "manuf:  0x%x\n" \   
    "oem:    %s\n" \     
    "prod:   %s\n" \     
    "rev:    %x\n" \     
    "serial: 0x%lx\n" \   
    "date:   %u/%u\n" \  
    "size:   %u MB\n" \
    "free:   %lu/%lu\n" \
    "copy:   %u\n" \     
    "wr.pr.: %u/%u\n" \  
    "format: %u\n",   
    disk_info.manufacturer,          
    (char*) disk_info.oem,           
    (char*) disk_info.product,       
    disk_info.revision,              
    disk_info.serial,                
    disk_info.manufacturing_month,    
    disk_info.manufacturing_year,    
    disk_info.capacity / 1024 / 1024,                
    diskFree>>16,
    diskSize>>16,
    disk_info.flag_copy,             
    disk_info.flag_write_protect_temp,
    disk_info.flag_write_protect,    
    disk_info.format);            
    
    return buffer;
}

#if FAT_DATETIME_SUPPORT
void WaspSD::get_datetime(uint16_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* min, uint8_t* sec)
{
    *year = 2007;
    *month = 1;
    *day = 1;
    *hour = 0;
    *min = 0;
    *sec = 0;
}
#endif

/*
 * DOS_cd changes directory, answers 0 if error
 */
uint8_t WaspSD::cd(struct fat_dir_entry_struct subdir_entry)
{
    struct fat_fs_struct* _fs;
    _fs=fs;
    
    struct fat_dir_struct* dd_new = fat_open_dir(_fs, &subdir_entry);
    if(dd_new)
    {
        fat_close_dir(dd);
        dd = dd_new;
        return 1;
    }
    return 0;
}

/*
 * DOS_cd changes directory, answers 0 if error
 */
uint8_t WaspSD::cd(const char* command)
{
    struct fat_fs_struct* _fs;
    _fs=fs;
    
    uint8_t exit=2;
    while(exit>0)
    {
        // check if the card is there or not
        if (!isSD())
        {
            flag = CARD_NOT_PRESENT;
            sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
            return 0;
        }
        struct fat_dir_entry_struct subdir_entry;
        if(find_file_in_dir(command, &subdir_entry))
        {
            struct fat_dir_struct* dd_new = fat_open_dir(_fs, &subdir_entry);
            if(dd_new)
            {
                fat_close_dir(dd);
                dd = dd_new;
                return 1;
            }
        }
        exit--;
        if(!exit) return 0;
    }
}

/*
 * ls ( offset, scope, info ) - directory listing
 *
 * returns and stores in DOS.buffer the listing of the current directory (DS.dd)
 * where the pointer is located. It has three parameters:
 *
 * - offset: it jumps over "offset" filenames in the list
 * - scope: it includes a total of "scope" filenames in the DOS.buffer
 * - info: limits the amount of information to be sent back, ranges from NAMES, SIZES, to ATTRIBUTES
 *
 * There is a limitation in size, due to DOS_BUFFER_SIZE. If the directory listing
 * was bigger than that, the function will include the characters ">>" at the end
 * and activate the TRUNCATED_DATA value in the DOS.flag. It is recommened to
 * check this value to assure data integrity.
 *
 * Examples of calls to ls(int, int, byte) are:
 *
 * - DOS.ls(2,0, NAMES): lists the name of the files 2nd, 3rd, 4th ... up to the end of the directory
 * - DOS.ls(4,2, SIZES): lists two files from position 4 including the size
 *
 * The information is sent back as a string where each file is listed in one line, an
 * example of this would be:
 *
 * empty.txt    2945
 * hola.txt     1149
 * secret/      0
 *
 * Files are shown with their extention, while folders have a slash as the last character. The
 * string is encoded with EOL ('\n'), EOS ('\0'), and tabulators ('\t') in case it needs further
 * parsing. The file size is shown in bytes.
 *
 */
char* WaspSD::ls(int offset, int scope, uint8_t info = NAMES)
{
    struct fat_fs_struct* _fs;
    struct fat_dir_struct* _dd;
    _fs=fs;
    _dd=dd;
    
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return buffer;
  }
    
  // if scope is zero, then we should make it 
  // ridiculously big, so that it goes on reading forever
  if (scope <= 0) scope = 1000;
  
  flag &= ~(TRUNCATED_DATA);
  struct fat_dir_entry_struct dir_entry;
  
  // clean the offset (directory listings before the one requested)
  if (offset-- > 0) while(fat_read_dir(_dd, &dir_entry) && offset > 0) 
  {
      offset--;
  }

  // declare the variables needed to iterate the listings
  int buff_count = 0, j = 0, buff_size = 30, BUFF_SIZE = 30, dos_buff_left = DOS_BUFFER_SIZE;
  
  // iterate the directory
  // if there is a directory entry, then create a string out of it
  // and add it to the buffer
  while(fat_read_dir(_dd, &dir_entry) && scope > 0)
  {
    buff_size = BUFF_SIZE;
    
    // create the line variable
    char line[BUFF_SIZE];
    
    // fill in the variable with the corresponding string
    switch (info)
    {
      case NAMES:
        sprintf(line, "%s\n", dir_entry.long_name);
        break;
      case SIZES:
        sprintf(line, "%s%c\t%lu\n", dir_entry.long_name, dir_entry.attributes & FAT_ATTRIB_DIR ? '/' : ' ', dir_entry.file_size);
        break;
      case ATTRIBUTES:
        sprintf(line, "%c%c %s\t%lu\n", dir_entry.attributes & FAT_ATTRIB_DIR ? 'd' : '-', dir_entry.attributes & FAT_ATTRIB_READONLY ? 'r' : 'w', dir_entry.long_name, dir_entry.file_size);
        break;
      default:
        // by default print only filenames, like the linux "ls" command
        sprintf(line, "%s\n", dir_entry.long_name);
        break;
    }
    
    // add the line to the buffer (this won't work with another sprintf, libc-avr goes bananas)
    for (j=0; j < BUFF_SIZE; j++) 
    {
      if (j + buff_count < DOS_BUFFER_SIZE - 4) 
      {
        if (line[j] != '\0') buffer[j + buff_count] = line[j];
      
        // in case you reach end of string, jump off the loop
        if (buffer[j + buff_count] == '\n' || line[j] == '\0') 
        {
          buff_size = j + buff_count + 1;
          //continue;
          j = BUFF_SIZE;  // dirty way of leaving the loop
        }
      } 
      else
      {
        // in case we filled up the whole buffer, we add a
        // visual end of buffer indicator and leave the loop
        buffer[DOS_BUFFER_SIZE - 4] = '>';
        buffer[DOS_BUFFER_SIZE - 3] = '>';
        buffer[DOS_BUFFER_SIZE - 2] = '\n';
        buffer[DOS_BUFFER_SIZE - 1] = '\0';
        flag |= TRUNCATED_DATA; 

        // go to the end of the directory
        while(fat_read_dir(_dd, &dir_entry)); 
        return buffer;  // leave the function here
      }
    }
    buff_count = buff_size;
    scope--;
  }

  // add an end of string to the buffer
  buffer[buff_count] = '\0';
  
  // go to the end of the directory
  while(fat_read_dir(_dd, &dir_entry)); 

  // return the buffer as long as there is any
  // data inside, zero if the directory was empty or
  // if there was an error
  if (buff_count > 0) return buffer;
  return 0;
}

/*
 * ls ( offset ) - directory listing
 *
 * returns and stores in DOS.buffer the listing of one file within the current 
 * directory (DS.dd) where the pointer is located. It has one parameter:
 *
 * - offset: it jumps over "offset" filenames in the list
 *
 * There is a limitation in size, due to DOS_BUFFER_SIZE. If the directory listing
 * was bigger than that, the function will include the characters ">>" at the end
 * and activate the TRUNCATED_DATA value in the DOS.flag. It is recommened to
 * check this value to assure data integrity.
 *
 * Examples of calls to ls(int) are:
 *
 * - DOS.ls(0): lists the name of the first file in the current directory
 * - DOS.ls(7): lists the name of the 7th file in the current directory
 *
 * The information is sent back as a string where each file is listed in one line, an
 * example of this would be:
 *
 * empty.txt    2945
 * 
 * (for a file)
 *
 * secret/      0
 *
 * (for a directory)
 *
 * Files are shown with their extention, while folders have a slash as the last character. The
 * string is encoded with EOL ('\n'), EOS ('\0'), and tabulators ('\t') in case it needs further
 * parsing. The file size is shown in bytes.
 *
 */
char* WaspSD::ls(int offset)
{
  return ls(offset, 1, SIZES);  // call DOS_ls with just the offset parameter
}

/*
 * ls ( void ) - directory listing
 *
 * returns and stores in DOS.buffer the full listing of the current directory (DS.dd)
 * where the pointer is located. It has no parameters
 *
 * There is a limitation in size, due to DOS_BUFFER_SIZE. If the directory listing
 * was bigger than that, the function will include the characters ">>" at the end
 * and activate the TRUNCATED_DATA value in the DOS.flag. It is recommened to
 * check this value to assure data integrity.
 *
 * An example of calls to ls(void) is:
 *
 * - DOS.ls(): lists the name of all the files up to the end of the directory
 *
 * The information is sent back as a string where each file is listed in one line, an
 * example of this would be:
 *
 * empty.txt    2945
 * hola.txt     1149
 * secret/      0
 *
 * Files are shown with their extention, while folders have a slash as the last character. The
 * string is encoded with EOL ('\n'), EOS ('\0'), and tabulators ('\t') in case it needs further
 * parsing. The file size is shown in bytes.
 *
 */
char* WaspSD::ls(void)
{
  return ls(0,0, SIZES);    // calls DOS_ls to list files until filling the buffer
}

/*
 * find_file_in_dir (name, dir_entry) - tests existance of files in the dd folder
 *
 * answers whether the file or directory "name" is available in the current directory
 *
 * If the file is available in the folder, it will answer 1 (TRUE), if not
 * available it will answer 0 (FALSE)
 *
 */
uint8_t WaspSD::find_file_in_dir(const char* name, struct fat_dir_entry_struct* dir_entry)
{
    struct fat_dir_struct* _dd;
    _dd=dd;
  // check if the card is there or not
    if (!isSD())
    {
        flag = CARD_NOT_PRESENT;
        sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
        return 0;
    }

    while(fat_read_dir(_dd, dir_entry))
    {
        if(strcmp(dir_entry->long_name, name) == 0)
        {
            fat_reset_dir(_dd);
            return 1;
        }
    }

    return 0;
}

/*
 * isFile (filename) - tests existence of files in the current folder
 *
 * answers whether the file "filename" is available in the current directory
 *
 * If the file is available in the folder, it will answer 1 (TRUE), if it is not
 * a file it will answer 0 (FALSE), and if it is not available it will answer -1 (ERROR)
 *
 * Assumes there is a valid filesystem pointer (DOS.fs), and directory pointer (DOS.dd)
 */
int8_t WaspSD::isFile(const char* filename)
{
    struct fat_dir_entry_struct file_entry;
    if(find_file_in_dir(filename,&file_entry)) 
    {
        return file_entry.attributes & FAT_ATTRIB_DIR ? 0 : 1;
    }
    return -1;
}

/*
 * openFile (filename) - opens a file
 *
 * opens the file "filename" if available and assigns the fp pointer to it, in the current directory
 *
 * If the file is not available in the folder, it will answer 0 (FALSE), it will also
 * update the DOS.flag to FILE_OPEN_ERROR
 *
 */
struct fat_file_struct* WaspSD::openFile(const char* filename)
{

    struct fat_fs_struct* _fs;
    struct fat_dir_struct* _dd;
    _fs=fs;
    _dd=dd;
    struct fat_file_struct* _fd;
    _fd=fd;
    
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    flag |= FILE_OPEN_ERROR;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return 0;
  }

  flag &= ~(FILE_OPEN_ERROR);
  struct fat_dir_entry_struct file_entry;
    if(!find_file_in_dir(filename,&file_entry))
    {
        flag |= FILE_OPEN_ERROR;
        return 0;
    }

    _fd=fat_open_file(_fs,&file_entry);
    if(!_fd) return fat_open_file(_fs,&file_entry);
    return _fd;
}

/*
 * closeFile (_fd) - closes a file
 *
 * closes the pointer "_fd" which was pointing to a file
 *
*/
void WaspSD::closeFile (struct fat_file_struct* _fd)
{
    fat_close_file(_fd);
}

/*
 * getFileSize (name) - answers the file size for filename in current folder
 *
 * answers a longint with the file "name" size in the current folder
 *
 * If the file is not available in the folder, it will answer -1, it will also
 * update the DOS.flag to FILE_OPEN_ERROR
 */
int32_t WaspSD::getFileSize(const char* name)
{
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    flag |= FILE_OPEN_ERROR;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return -1;
  }

  flag &= ~(FILE_OPEN_ERROR);
  struct fat_dir_entry_struct file_entry;
    if(!find_file_in_dir(name,&file_entry))
    {
        flag |= FILE_OPEN_ERROR;
        return -1;
    }

    return file_entry.file_size;
}

/*
 * getAttributes (name) - returns the attributes for a directory or file entry 
 *
 * returns the attributes for a directory or file entry in the current directory. The attributes
 * are encoded with two characters:
 *
 * - char #1: it is either "d" for a directory or "-" for a file entry
 * - char #2: is either "r" for read only, and "w" if the file/directory is also writeable
 *
 * If the file or directory is not available in the folder, it will answer "--", it will also
 * update the DOS.flag to FILE_OPEN_ERROR
 */
char* WaspSD::getAttributes(const char* name)
{
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    flag |= FILE_OPEN_ERROR;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return buffer;
  }

  flag &= ~(FILE_OPEN_ERROR);
  struct fat_dir_entry_struct file_entry;
    if(!find_file_in_dir(name,&file_entry))
    {
        flag |= FILE_OPEN_ERROR;
        return "--";
    }

    char attrib[2];
    sprintf(attrib, "%c%c", file_entry.attributes & FAT_ATTRIB_DIR ? 'd' : '-', file_entry.attributes & FAT_ATTRIB_READONLY ? 'r' : 'w');

    return attrib;
}

/*
 * cat (filename, offset, scope) 
 *
 * dumps into the DOS.buffer the amount of bytes in scope after offset 
 * coming from filename, it will also return it as a string
 *
 * There is a limitation in size, due to DOS_BUFFER_SIZE. If the data read
 * was bigger than that, the function will include the characters ">>" at the end
 * and activate the TRUNCATED_DATA value in the DOS.flag. It is recommened to
 * check this value to assure data integrity.
 *
 * If there was an error opening the file, the returned string will say so and
 * the DOS.flag will show the FILE_OPEN_ERROR bit active
 *
 * An example of calls to cat(filename, offset, scope) is:
 *
 * - DOS.cat("hola.txt", 3, 17): will show the 17 characters after jumping over 3 in the file "hola.txt"
 *
 * The information is sent back as a string where each one of the characters are 
 * printed one after the next, EOL ('\n') will be encoded as EOL, and will be
 * accounted as one byte, an example of this would be:
 *
 * un lugar
 * de la man
 *
 */
char* WaspSD::cat (const char* filename, int32_t offset, uint16_t scope)
{
    struct fat_file_struct* _fd;
    _fd=fd;
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    flag |= FILE_OPEN_ERROR;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return buffer;
  }
    
  // if scope is zero, then we should make it 
  // ridiculously big, so that it goes on reading forever
  if (scope <= 0) scope = 1000;
  
  flag &= ~(TRUNCATED_DATA | FILE_OPEN_ERROR);

  // search file in current directory and open it 
  // assign the file pointer to the general file pointer "fp"
  // exit if error and modify the general flag with FILE_OPEN_ERROR
  _fd = openFile(filename);
  if(!_fd)
  {
    sprintf(buffer, "error opening %s", filename);
    flag |= FILE_OPEN_ERROR;
    return buffer;
  }

  // iterate through the file
  byte maxBuffer = 10;  // size of the buffer to use when reading
  uint8_t bufferSD[maxBuffer];
  uint32_t cont = 0;
  
  // first jump over the offset
  if(!fat_seek_file(_fd, &offset, FAT_SEEK_SET))
  {
	  sprintf(buffer, "error seeking on: %s\n", filename);
	  fat_close_file(_fd);
	  return buffer;
  }
  
  uint8_t readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));

  // second, read the data and store it in the DOS.buffer
  // as long as there is room in it
  while(readRet > 0 && scope > 0 && cont < DOS_BUFFER_SIZE)
  {
    for(uint8_t i = 0; i < readRet; ++i)
    {
      buffer[cont++] = bufferSD[i];
      scope--;
      if( scope <=0 ) break;
      if( cont>=DOS_BUFFER_SIZE ) break;
    }
    if( readRet<maxBuffer ) break;
    readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
  }
  if (cont < DOS_BUFFER_SIZE - 1) {
    buffer[cont++] = '\0';
  }
  else
  {
    // in case we filled up the whole buffer, we add a
    // visual end of buffer indicator and leave the loop
    buffer[DOS_BUFFER_SIZE - 4] = '>';
    buffer[DOS_BUFFER_SIZE - 3] = '>';
    buffer[DOS_BUFFER_SIZE - 2] = '\n';
    buffer[DOS_BUFFER_SIZE - 1] = '\0';
    flag |= TRUNCATED_DATA; 
  }

  fat_close_file(_fd);

  return buffer;

}

/*
 * cat (filename, offset, scope) 
 *
 * dumps into the DOS.buffer the amount of bytes in scope after offset 
 * coming from filename, it will also return it as a string
 *
 * There is a limitation in size, due to DOS_BUFFER_SIZE. If the data read
 * was bigger than that, the function will include the characters ">>" at the end
 * and activate the TRUNCATED_DATA value in the DOS.flag. It is recommened to
 * check this value to assure data integrity.
 *
 * If there was an error opening the file, the returned string will say so and
 * the DOS.flag will show the FILE_OPEN_ERROR bit active
 *
 * An example of calls to cat(filename, offset, scope) is:
 *
 * - DOS.cat("hola.txt", 3, 17): will show the 17 characters after jumping over 3 in the file "hola.txt"
 *
 * The information is sent back as a string where each one of the characters are 
 * printed one after the next, EOL ('\n') will be encoded as EOL, and will be
 * accounted as one byte, an example of this would be:
 *
 * un lugar
 * de la man
 *
 */
uint8_t* WaspSD::catBin (const char* filename, int32_t offset, uint16_t scope)
{
	struct fat_file_struct* _fd;
	_fd=fd;
  // check if the card is there or not
	if (!isSD())
	{
		flag = CARD_NOT_PRESENT;
		flag |= FILE_OPEN_ERROR;
		sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
		return bufferBin;
	}
    
  // if scope is zero, then we should make it 
  // ridiculously big, so that it goes on reading forever
	if (scope <= 0) scope = 1000;
  
	flag &= ~(TRUNCATED_DATA | FILE_OPEN_ERROR);

  // search file in current directory and open it 
  // assign the file pointer to the general file pointer "fp"
  // exit if error and modify the general flag with FILE_OPEN_ERROR
	_fd = openFile(filename);
	if(!_fd)
	{
		sprintf(buffer, "error opening %s", filename);
		flag |= FILE_OPEN_ERROR;
		return bufferBin;
	}

  // iterate through the file
	byte maxBuffer = 1;  // size of the buffer to use when reading
	uint8_t bufferSD[maxBuffer];
	uint32_t cont = 0;
  
  // first jump over the offset
	if(!fat_seek_file(_fd, &offset, FAT_SEEK_SET))
	{
		sprintf(buffer, "error seeking on: %s\n", filename);
		fat_close_file(_fd);
		return bufferBin;
	}
  
	uint8_t readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));

  // second, read the data and store it in the DOS.buffer
  // as long as there is room in it
	while(readRet > 0 && scope > 0 && cont < BIN_BUFFER_SIZE)
	{
		for(uint8_t i = 0; i < maxBuffer; ++i)
		{
			bufferBin[cont++] = bufferSD[i];
			scope--;
			readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
		}
	}

	fat_close_file(_fd);

	return bufferBin;

}

/*
 * catln (filename, offset, scope) 
 *
 * dumps into the DOS.buffer the amount of lines in scope after offset 
 * lines coming from filename, it will also return it as a string
 *
 * There is a limitation in size, due to DOS_BUFFER_SIZE. If the data read
 * was bigger than that, the function will include the characters ">>" at the end
 * and activate the TRUNCATED_DATA value in the DOS.flag. It is recommened to
 * check this value to assure data integrity.
 *
 * If there was an error opening the file, the returned string will say so and
 * the DOS.flag will show the FILE_OPEN_ERROR bit active
 *
 * An example of calls to catln(filename, offset, scope) is:
 *
 * - DOS.catln("hola.txt", 10, 5): will show the 5 lines following line number 10 in the file "hola.txt"
 *
 * The information is sent back as a string where each one of the file's lines are 
 * printed in one line, an example of this would be:
 *
 * en un lugar
 * de la mancha de
 * cuyo nombre no qui>>
 *
 * Lines end with EOF ('\n'), this is the symbol used to count the amount of lines.
 */
char* WaspSD::catln (const char* filename, uint32_t offset, uint16_t scope)
{
    struct fat_file_struct* _fd;
    _fd=fd;
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    flag |= FILE_OPEN_ERROR;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return buffer;
  }
    
  // if scope is zero, then we should make it 
  // ridiculously big, so that it goes on reading forever
  if (scope <= 0) scope = 1000;
  
  flag &= ~(TRUNCATED_DATA | FILE_OPEN_ERROR);

  // search file in current directory and open it 
  // assign the file pointer to the general file pointer "fp"
  // exit if error and modify the general flag with FILE_OPEN_ERROR
  _fd = openFile(filename);
  if(!_fd)
  {
    sprintf(buffer, "error opening %s", filename);
    flag |= FILE_OPEN_ERROR;
    return buffer;
  }

  byte maxBuffer = 1;  // size of the buffer to use when reading
  uint8_t bufferSD[maxBuffer];
  uint32_t cont = 0;
  
  // jump over offset lines
  uint8_t readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
  if (offset > 0) 
  {
    while( readRet > 0 && offset > 0)   
    {
      for(uint8_t i = 0; i < maxBuffer; ++i)
      {
        if (bufferSD[i] == '\n')
          offset--;
      }
      readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
    }

    // do not forget the first buffer  
    for(uint8_t i = 0; i < maxBuffer; ++i)
    {
      buffer[cont++] = bufferSD[i];
      if (bufferSD[i] == '\n')
        scope--;
    }

    readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
  }
  
  // add to buffer scope lines
  while(readRet > 0 && scope > 0 && cont < DOS_BUFFER_SIZE)
  {
    for(uint8_t i = 0; i < maxBuffer; ++i)
    {
      buffer[cont++] = bufferSD[i];
      if (bufferSD[i] == '\n')
        scope--;
    }
    readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
  }

  // are we at the end of the buffer yet?
  if (cont < DOS_BUFFER_SIZE - 1) {
    buffer[cont++] = '\0';
  }
  else
  {
    // in case we filled up the whole buffer, we add a
    // visual end of buffer indicator and leave the loop
    buffer[DOS_BUFFER_SIZE - 4] = '>';
    buffer[DOS_BUFFER_SIZE - 3] = '>';
    buffer[DOS_BUFFER_SIZE - 2] = '\n';
    buffer[DOS_BUFFER_SIZE - 1] = '\0';
    flag |= TRUNCATED_DATA; 
  }

  fat_close_file(_fd);

  return buffer;

}

/*
 * indexOf ( filename, pattern, offset ) - search for first occurrence of a string in a file
 *
 * looks into filename for the first occurrence of the pattern after a certain offset. The
 * algorythm will jump over offset bytes before starting the search for the pattern. It will
 * returns the amount of bytes (as a longint) to the pattern from the offset
 *
 * Example, file hola.txt contains:
 *
 * hola caracola\nhej hej\n   hola la[EOF]
 *
 * The following table shows the results from searching different patterns:
 *
 * Command                            Answer
 * indexOf("hola.txt", "hola", 0)       0
 * indexOf("hola.txt", "hola", 1)       23
 * indexOf("hola.txt", "hej", 3)        11
 *
 * Notes:
 *
 * - the special characters like '\n' (EOL) are accounted as one byte
 * - files are indexed from 0 
 *
 * If there was an error opening the file, the buffer string will say so and
 * the DOS.flag will show the FILE_OPEN_ERROR bit active
 */
int32_t WaspSD::indexOf (const char* filename, const char* pattern, uint32_t offset)
{
    struct fat_file_struct* _fd;
    _fd=fd;
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    flag |= FILE_OPEN_ERROR;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return -1;
  }

  flag &= ~(FILE_OPEN_ERROR);

  // search file in current directory and open it 
  // assign the file pointer to the general file pointer "fp"
  // exit if error and modify the general flag with FILE_OPEN_ERROR
  _fd = openFile(filename);
  if(!_fd)
  {
    sprintf(buffer, "error opening %s", filename);
    flag |= FILE_OPEN_ERROR;
    return -1;
  }

  uint32_t limitSize = getFileSize(filename);

  uint8_t limitPattern = Utils.sizeOf(pattern);
  uint8_t bufferSD[1];
  uint8_t* cmpPattern = (uint8_t*) calloc(limitPattern,sizeof(uint8_t));
  if( cmpPattern==NULL ) return -1;
  uint8_t exit = 0, contPattern = 0;
  uint32_t cont = 0;

  // jump over the offset
  uint8_t readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
  if (offset > 0) while(readRet > 0 && offset > 0) 
  { 
    readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
    offset--; 
    //cont++;  // we don't wanna count until we are done with the offset
  }

  // fill in the first buffer
  for(int j=0; j < limitPattern - 1; j++)
    if (readRet > 0)
    {
      cmpPattern[j] = bufferSD[0];
      readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
    }
    else
      exit = 1;  // exit if the filesize is smaller than the pattern

//to debug the code in the library it is possible to use the buffer together with sprintf, e.g.
//sprintf(buffer,"%s - %c%c%c%c - %lu - %u -%lu", pattern, cmpPattern[0], cmpPattern[1], cmpPattern[2], bufferSD[0], cont, limitPattern, limitSize);

  // run inside the buffer
  while(readRet > 0 && cont < limitSize && !exit)
  {
    // take the last position in the buffer
    cmpPattern[limitPattern-1] = bufferSD[0];
    
    // compare the strings, exit if found
//    if(strcmp((const char*) pattern, (const char*) cmpPattern) == 0)
    if(Utils.strCmp((const char*) pattern, (const char*) cmpPattern, limitPattern) == 0)
      exit = 1;
      
    // if not exit, increment counter
    if (!exit) 
      cont++;
      
    // shift cmpPattern to the left one position
    for(int j = 0; j < limitPattern - 1; j++) cmpPattern[j] = cmpPattern[j+1];
    
    // read the next buffer
    readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
  }

  fat_close_file(_fd);

  free(cmpPattern);
  // in case we checked the whole buffer, we return error
  if (cont >= limitSize - limitPattern ) 
    return -1; 

  // otherwise we return the pattern's location
  return cont;

}

/*
 * isDir ( dir_entry ) - answers whether a file is a dir or not
 *
 * returns 1 if it is a dir, 0 if error, will
 * not modify any flags
 */
uint8_t WaspSD::isDir(struct fat_dir_entry_struct dir_entry)
{
    return dir_entry.attributes & FAT_ATTRIB_DIR ? 1 : 0;
}

/*
 * isDir ( dirname ) - tests the existence of a directory in the current directory
 * and checks if it is adirectory or no
 *
 * returns 1 if it exists and it is a dir, 0 if exists but it is not a dir, -1 if error
 * will not modify any flags
 */
int8_t WaspSD::isDir(const char* dirname)
{
    struct fat_dir_entry_struct dir_entry;
    if(find_file_in_dir(dirname,&dir_entry)) 
    {
        return dir_entry.attributes & FAT_ATTRIB_DIR ? 1 : 0;
    }
    return -1;
}

/*
 * delFile ( file_entry ) - answers whether a file is deleted or not
 *
 * returns 1 if file_entry is deleted via fat command, 0 if error, will
 * not modify any flags
 */
uint8_t WaspSD::delFile(struct fat_dir_entry_struct file_entry)
{
    struct fat_fs_struct* _fs;
    _fs=fs;
    if(fat_delete_file(_fs,&file_entry)) return 1;
    return 0;
}

/*
 * delDir ( depth ) - answers whether a dir is deleted or not
 *
 * returns 1 if fileentry is deleted via iterative fat commands, 0 if error, will
 * not modify any flags
 * 'depth' is still not used, but function supports developers actualizations
 */
uint8_t WaspSD::delDir(uint8_t depth)
{
  uint8_t exit = 1;
  struct fat_dir_entry_struct dir_entry;
  struct fat_dir_struct* _dd;
  struct fat_fs_struct* _fs;
  _fs=fs;
  _dd = dd;
  while(fat_read_dir(_dd, &dir_entry))
  {
    if (isDir(dir_entry) && !(Utils.strCmp(dir_entry.long_name,".\0",2) == 0 || Utils.strCmp(dir_entry.long_name,"..\0",3) == 0)) exit = 0;
  }
  if (exit) while(fat_read_dir(_dd, &dir_entry))
  {
    if (!(Utils.strCmp(dir_entry.long_name,".\0",2) == 0 || Utils.strCmp(dir_entry.long_name,"..\0",3) == 0)) 
    {
      exit &= fat_delete_file(_fs, &dir_entry);
    }
  }
  return exit;
}

/*
 * del ( name ) - delete file or directory
 *
 * returns 1 if it is possible to delete "name", 0 if error, will
 * not modify any flags
 *
 * Version 1a, as for 20090512 allows only erasing depth one directories, thus
 * if the user calls to erase a directory with subdirs, it will exit with error
 * without erasing the directory
 *
 * It also allows erasing current directory "." under the same premises: it should
 * contain no subdirectories or it will exit with error
 *
 * Thanks to this function, together with delFile, delDir and isDir it is possible to
 * create more complex delete functions that could iterate through any directory structure
 */
uint8_t WaspSD::del(const char* name)
{
    
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return 0;
  }

  // first case: erase the contents in current directory
  if (Utils.strCmp(name,".\0",2) == 0)
  {
    return delDir(0);
  }
  
  // second case: erase whatever else
  struct fat_dir_entry_struct file_entry;
  if(find_file_in_dir(name,&file_entry))
  {
    if (isDir(file_entry))
    {
      if(!(cd(file_entry))) return 0;
      uint8_t aux = delDir(0);
      if(!(cd(".."))) return 0;
      if (aux) 
      {
        aux &= delFile(file_entry);
      }
      return aux;
    }
    else
      return delFile(file_entry);
  }
  
  // if you made this far, it gotta be an error
  return 0;
}

/*
 * create ( filename ) - create file
 *
 * returns 1 on file creation, 0 if error, will mark the flag with
 * FILE_CREATION_ERROR 
 */
uint8_t WaspSD::create(const char* filename)
{
    struct fat_dir_struct* _dd;
    _dd=dd;
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    flag |= FILE_CREATION_ERROR;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return 0;
  }

  flag &= ~(FILE_CREATION_ERROR);
  struct fat_dir_entry_struct file_entry;
  if(!fat_create_file(_dd, filename, &file_entry))
  {
      flag |= FILE_CREATION_ERROR;
      return 0;
  }
  return 1;
}

/*
 * mkdir ( dirname ) - create directory
 *
 * returns 1 on directory creation, 0 if error, will mark the flag with
 * DIR_CREATION_ERROR 
 */
uint8_t WaspSD::mkdir(const char* dirname)
{
    struct fat_dir_struct* _dd;
    _dd=dd;
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    flag |= DIR_CREATION_ERROR;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return 0;
  }

  flag &= ~(DIR_CREATION_ERROR);
  struct fat_dir_entry_struct dir_entry;
  if( (find_file_in_dir(dirname,&dir_entry)) || (!fat_create_dir(_dd, dirname, &dir_entry)) )
  {
      flag |= DIR_CREATION_ERROR;
      return 0;
  }
  return 1;
}

/*
 * numFiles ( void ) - returns the amount of files in dir
 *
 * returns the amount of files in the current directory
 * a negative answer indicates error, zero means no files in the folder
 */
int8_t WaspSD::numFiles()
{
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return -1;
  }

  uint8_t cont = -2;
  const char* command=".";
  struct fat_dir_entry_struct dir_entry;
  if(!(cd(command))) cont=0;
  while(fat_read_dir(dd, &dir_entry))
  {
    cont++;
  }

  return cont;
}

/*
 * append ( filename, str ) - write strings at the end of files
 *
 * writes the string "str" at the end of the file "filename"
 *
 * returns 1 on success, 0 if error, will mark the flag with
 * FILE_WRITING_ERROR
 */
uint8_t WaspSD::append(const char* filename, const char* str)
{
    return writeSD(filename, str, getFileSize(filename));
}

/*
 * append ( filename, str, length ) - write strings at the end of files
 *
 * writes the string "str" at the end of the file "filename"
 *
 * returns 1 on success, 0 if error, will mark the flag with
 * FILE_WRITING_ERROR
 */
uint8_t WaspSD::append(const char* filename, const char* str, uint16_t length)
{
	return writeSD(filename, str, getFileSize(filename), length);
}


/*
 * append ( filename, str ) - write array of numbers at the end of files
 *
 * writes the array of numbers "str" at the end of the file "filename"
 *
 * returns 1 on success, 0 if error, will mark the flag with
 * FILE_WRITING_ERROR
 */
uint8_t WaspSD::append(const char* filename, uint8_t* str)
{
	return writeSD(filename, str, getFileSize(filename));
}

/*
 * appendln ( filename, str ) - write strings at the end of files
 *
 * writes the string "str" at the end of the file "filename" adding end
 * of line
 *
 * returns 1 on success, 0 if error, will mark the flag with
 * FILE_WRITING_ERROR
 */
uint8_t WaspSD::appendln(const char* filename, const char* str)
{
    uint8_t exit = 0;
    exit = append(filename, str);
#ifndef FILESYSTEM_LINUX
    if (exit) exit &= append(filename, "\r");
#endif
    if (exit) exit &= append(filename, "\n");
    return exit;
}

/*
 * appendln ( filename, str ) - write array of numbers at the end of files
 *
 * writes the array of numbers "str" at the end of the file "filename" adding end
 * of line
 *
 * returns 1 on success, 0 if error, will mark the flag with
 * FILE_WRITING_ERROR
 */
uint8_t WaspSD::appendln(const char* filename, uint8_t* str)
{
	uint8_t exit = 0;
	exit = append(filename, str);
#ifndef FILESYSTEM_LINUX
	if (exit) exit &= append(filename, "\r");
#endif
	if (exit) exit &= append(filename, "\n");
	return exit;
}

/*
 * writeSD ( filename, str, offset ) - write strings to files
 *
 * writes the string "str" to the file "filename" after a certain "offset"
 *
 * returns 1 on success, 0 if error, will mark the flag with
 * FILE_WRITING_ERROR
 */
uint8_t WaspSD::writeSD(const char* filename, const char* str, int32_t offset)
{
    struct fat_file_struct* _fd;
    _fd=fd;
  // check if the card is there or not
    if (!isSD())
    {
        flag = CARD_NOT_PRESENT;
        flag |= FILE_WRITING_ERROR;
        sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
        return 0;
    }

    flag &= ~(FILE_WRITING_ERROR);
    uint8_t exit = 0;
  // search file in current directory and open it
    _fd = openFile(filename);
    if(!_fd)
    {
        sprintf(buffer, "error opening: %s\n", filename);
        exit = 1;
    }

    if(!exit) if(!fat_seek_file(_fd, &offset, FAT_SEEK_SET))
    {
        sprintf(buffer, "error seeking on: %s\n", filename);
        fat_close_file(_fd);
        exit = 1;
    }

    uint16_t data_len = Utils.sizeOf(str);

  // write text to file
    if(!exit) if(fat_write_file(_fd, (uint8_t*) str, data_len) != data_len)
    {
        sprintf(buffer, "error writing to: %s\n", filename);
        fat_close_file(_fd);
        exit = 1;
    }

    if(!exit) fat_close_file(_fd);
    else flag |= FILE_WRITING_ERROR;
    return !exit;
}

/*
 * writeSD ( filename, str, offset ) - write numbers to files
 *
 * writes the aray of integers "str" to the file "filename" after a certain "offset"
 *
 * returns 1 on success, 0 if error, will mark the flag with
 * FILE_WRITING_ERROR
 */
uint8_t WaspSD::writeSD(const char* filename, uint8_t* str, int32_t offset)
{
	struct fat_file_struct* _fd;
	_fd=fd;
  // check if the card is there or not
	if (!isSD())
	{
		flag = CARD_NOT_PRESENT;
		flag |= FILE_WRITING_ERROR;
		sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
		return 0;
	}

	flag &= ~(FILE_WRITING_ERROR);
	uint8_t exit = 0;
  // search file in current directory and open it
	_fd = openFile(filename);
	if(!_fd)
	{
		sprintf(buffer, "error opening: %s\n", filename);
		exit = 1;
	}

	if(!exit) if(!fat_seek_file(_fd, &offset, FAT_SEEK_SET))
	{
		sprintf(buffer, "error seeking on: %s\n", filename);
		fat_close_file(_fd);
		exit = 1;
	}

	uint16_t data_len = 0;
	while( (str[data_len]!=0xAA) || (str[data_len+1]!=0xAA) ) data_len++;

  // write text to file
	if(!exit) if(fat_write_file(_fd, str, data_len) != data_len)
	{
		sprintf(buffer, "error writing to: %s\n", filename);
		fat_close_file(_fd);
		exit = 1;
	}

	if(!exit) fat_close_file(_fd);
	else flag |= FILE_WRITING_ERROR;
	return !exit;
}


/*
 * writeSD ( filename, str, offset, length ) - write strings to files
 *
 * writes the string "str" to the file "filename" after a certain "offset"
 *
 * returns 1 on success, 0 if error, will mark the flag with
 * FILE_WRITING_ERROR
 */
uint8_t WaspSD::writeSD(const char* filename, const char* str, int32_t offset, int16_t length)
{
	struct fat_file_struct* _fd;
	_fd=fd;
  // check if the card is there or not
	if (!isSD())
	{
		flag = CARD_NOT_PRESENT;
		flag |= FILE_WRITING_ERROR;
		sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
		return 0;
	}

	flag &= ~(FILE_WRITING_ERROR);
	uint8_t exit = 0;
  // search file in current directory and open it
	_fd = openFile(filename);
	if(!_fd)
	{
		sprintf(buffer, "error opening: %s\n", filename);
		exit = 1;
	}

	if(!exit) if(!fat_seek_file(_fd, &offset, FAT_SEEK_SET))
	{
		sprintf(buffer, "error seeking on: %s\n", filename);
		fat_close_file(_fd);
		exit = 1;
	}

  // write text to file
	if(!exit) if(fat_write_file(_fd, (uint8_t*) str, length) != length)
	{
		sprintf(buffer, "error writing to: %s\n", filename);
		fat_close_file(_fd);
		exit = 1;
	}

	if(!exit) fat_close_file(_fd);
	else flag |= FILE_WRITING_ERROR;
	return !exit;
}


/*
 * numln ( filename ) - returns the amount of lines in file
 *
 * returns the amount of lines in "filename" that should be in the current directory,
 * a negative answer indicates error, zero means no lines in the file
 *
 * This method counts the occurrence of the character '\n' in the file. If there
 * was a problem opening it, the FILE_OPEN_ERROR would be activated and will return -1
 */
int32_t WaspSD::numln(const char* filename)
{
    struct fat_file_struct* _fd;
    _fd=fd;
  // check if the card is there or not
  if (!isSD())
  {
    flag = CARD_NOT_PRESENT;
    flag |= FILE_OPEN_ERROR;
    sprintf(buffer,"%s", CARD_NOT_PRESENT_em);
    return -1;
  }
    
  flag &= ~(FILE_OPEN_ERROR);

  // search file in current directory and open it 
  // assign the file pointer to the general file pointer "fp"
  // exit if error and modify the general flag with FILE_OPEN_ERROR
  _fd = openFile(filename);
  if(!_fd)
  {
    sprintf(buffer, "error opening %s", filename);
    flag |= FILE_OPEN_ERROR;
    return -1;
  }

  byte maxBuffer = 1;  // size of the buffer to use when reading
  uint8_t bufferSD[maxBuffer];
  uint32_t cont = 0;
  
  // jump over offset lines
  uint8_t readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
  while( readRet > 0)   
  {
    for(uint8_t i = 0; i < maxBuffer; ++i)
    {
      if (bufferSD[i] == '\n')
        cont++;
    }
    readRet = fat_read_file(_fd, bufferSD, sizeof(bufferSD));
  }

  fat_close_file(_fd);

  return cont;

}

// Utils ////////////////////////////////////////////////////////////////////

/*
 * cleanFlags ( void ) - resets all the flags, returns the flags
 */
uint16_t WaspSD::cleanFlags(void)
{
  flag = 0;
  return flag;
}


// Private Methods /////////////////////////////////////////////////////////////

// Preinstantiate Objects //////////////////////////////////////////////////////

WaspSD SD = WaspSD();


