/**
 * 
 * #define NENTRIES (sizeof(struct dirent) / 512)
 * #define FREE 1
 * 
 * struct dirent dir[NENTRIES];
 * int isDirInMemory = 0;
 * 
 *  int create(char[8] name, int maxSize) {
 * 		int entry; uint block;
 * 
 * 		if !validName (name) return -EINVNAME;
 * 		if !isDirInMemory
 * 		{ readBlock(1, dir) ; isDirInMemory = 1 } 
 * 			if ((entry = lookupDir(name)) < 0) // retorna indíce entrada livre
 * 			if (entry == -1) return -EDIRFULL // directoria cheia
 * 			else return -EEXISTS // já existe ficheiro com esse nome
 * 		}
 * 
 * 	if !(block = getBlocks(roundUp(maxSize, 512), FREE))
 * 	return -ENOSPACE;
 * 
 * 	memcpy(dir[entry].nome, name, 8);
 * 	dir[entry].inicio = block;
 * 	dir[entry].comprActual = 0;
 * 	dir[entry].comprMaximo = roundUp(maxSize, 512);
 * 	range[0] = block; range[1] = comprMax; setBlock(range, !FREE);
 * 	writeBlock(dir, 1); updateFreeBlkInfo(...;)
 * 	return 0;
 */