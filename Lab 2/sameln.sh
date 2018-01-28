#!/bin/sh

D="$1"
cd $D

for FILE_1 in .* *	#include hidden files as well 
do
	for FILE_2 in .* *
	do	#Ignore when file is the same name as itself!
		if ["$FILE_1" == "$FILE_2"]; 
			then
			continue;
		fi
		#Ignore non-files
		if [! (-f "$FILE_1")];
			then
			continue;
		fi

		if [! (-f "$FILE_2")];
			then
			continue;
		fi

        # If not readable, print error and continue
		if [! (-r "$FILE_1")];
			then
			echo "$FILE_1 is not readable"
			continue;
		fi

		if [! (-r "$FILE_2")];
			then
			echo "$FILE_2 is not readable"
			continue;
		fi

		#Compare files now

		cmp -s "$FILE_1" "$FILE_2"
		if [$? == 0];	# Files are the same
			then
			if [${FILE_1:0:1} == '.'];
				then
				if [${FILE_2:0:1} != '.'];
					then
					rm "$FILE_2"
					ln "$FILE_1" "$FILE_2"
				fi
			elif [${FILE_2:0:1} == '.'];
				then
				if [${FILE_1:0:1} != '.'];
					then
					rm "$FILE_1"
					ln "$FILE_2" "$FILE_1"
				fi
			elif ["$FILE_1" < "$FILE_2"];
				then
				rm "$FILE_2"
				ln "$FILE_1" "$FILE_2"
			else
				rm "$FILE_1"
				ln "$FILE_2" "$FILE_1"
			fi
		fi
	done
done





