#!/usr/bin/env bash
# @description  shows file size histogram in required dir 
#               and shows histogram of type of files in dir
# @date         26.03.2017
# @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz)

POSIXLY_CORRECT="yes"                

export NORM_FORMA="false"            # -n
export REGEXP="false"                # -i
export ARG=" "                        # arguments to -i
export COLS_SIZE_N=1                 # number of columns
export cesta="."                    

# terminal -> output size of terminal
# file     -> max size 79
if [ -t 1 ] ; then
  COLS_SIZE_N=`tput cols`
  COLS_SIZE_N=$(($COLS_SIZE_N-1))
else
  COLS_SIZE_N=79
fi

# ------------------------------------------
# Info about path, number of dirs and files
# -----------------------------------------
function basic_info {

  if [ "$REGEXP"="true" ]; then
    number_of_dirs=$(find -type d | sed 's/.\///' |awk -v reg="$ARG" ' BEGIN{ FS = "/" } {
      a=0
      for (i=1; i<=NF; i++) {
        if ($i ~ reg )
         a=1
      }
      if (a != 1) {
        print $0
      }
    }' | wc -l)

    number_of_files=$(find -type f | sed 's/.\///' |awk -v reg="$ARG" ' BEGIN{ FS = "/" } {
      a=0
      for (i=1; i<=NF; i++) {
        if ($i ~ reg )
         a=1
      }
      if (a != 1) {
        print $0
      }
    }' | wc -l)
  else
    number_of_dirs=$(find -type d  | wc -l)
    number_of_files=$(find -type f | wc -l)
  fi

  printf "Root directory: "
  printf "%s\n" "$cesta"
  printf "Directories: "
  printf "%s\n" "$number_of_dirs"
  printf "All files: "
  printf "%s\n" "$number_of_files"

  return 0
}


# ---------------------------------_-------------
# Histogram acording filesize in dir and subdirs
# -----------------------------------------------
function hist_size() {

  if [ "$REGEXP"="true" ]; then
    all_files_ls=$(find -type f | sed 's/.\///' |awk -v reg="$ARG" ' BEGIN{ FS = "/" } {
      a=0
      for (i=1; i<=NF; i++) {
        if ($i ~ reg )
         a=1
      }
      if (a != 1) {
        print $0
      }
    }' | xargs ls -l |awk '{print $5}' )
  else
    all_files_ls=$(find -type f  -exec ls -l {} \; |awk '{print $5}' )
  fi

  for i in $all_files_ls ; do
    if [ $(($i)) -lt 100 ]          ; then   # < 100B
      pocet[1]=$((pocet[1]+1))
    elif [ $(($i)) -lt 1024 ]       ; then   # < 1 KiB
      pocet[2]=$((pocet[2]+1))
    elif [ $(($i)) -lt 10240 ]      ; then   # < 10 KiB
      pocet[3]=$((pocet[3]+1))
    elif [ $(($i)) -lt 102400 ]     ; then   # < 100 KiB
      pocet[4]=$((pocet[4]+1))
    elif [ $(($i)) -lt 1048576 ]    ; then   # < 1 MiB
      pocet[5]=$((pocet[5]+1))
    elif [ $(($i)) -lt 10485760 ]   ; then   # < 10 MiB
      pocet[6]=$((pocet[6]+1))
    elif [ $(($i)) -lt 10485760 ]   ; then   # < 100 MiB
      pocet[7]=$((pocet[7]+1))
    elif [ $(($i)) -lt 1073741824 ] ; then   # < 1 GiB
      pocet[8]=$((pocet[8]+1))
    else                                     # >= 1 GiB
      pocet[9]=$((pocet[9]+1))
    fi
  done


  # normalization
  if [ $NORM_FORMA = "true" ] ; then
    a=2
    max=1
    while [ $a -le $((${#pocet[@]})) ] ; do
      if [ $((${pocet[$max]})) -lt $((${pocet[$a]})) ] ; then
        max=$a
      fi
      a=$(($a+1))
    done

    if [ $((${pocet[$max]})) -gt $(($COLS_SIZE_N-12)) ]; then
      old_max=$((${pocet[$max]}))
      pocet[$max]=$(($COLS_SIZE_N-12))
      i=1
      while [ $(($i)) -le $((${#pocet[@]})) ]; do
        if [ $(($i)) -eq $(($max)) ]; then
          i=$(($i+1))
          continue
        fi
        pocet[$i]=$((  pocet[i] * pocet[max]  / old_max ))
        i=$(($i+1))
      done
    fi    
  fi       

  # output of 1. histogram
  index[1]="  <100 B  : "
  index[2]="  <1 KiB  : "
  index[3]="  <10 KiB : "
  index[4]="  <100 KiB: "
  index[5]="  <1 MiB  : "
  index[6]="  <10 MiB : "
  index[7]="  <100 MiB: "
  index[8]="  <1 GiB  : "
  index[9]="  >=1 GiB : "

  printf "File size histogram:\n"
  for i in ${!index[@]} ; do
    printf "${index[$i]}"
    j=1
    while [ $j -le $((pocet[$i])) ]; do
      printf "#"
      j=$((j+1))
    done
    printf "\n"
  done

  return 0
}


# -----------------------------------------
# Histogram acording occurence of file type
# -----------------------------------------
function hist_files() {

  if [ "$REGEXP"="true" ]; then
    pole=$(find -type f | sed 's/.\///' |awk -v reg="$ARG" ' BEGIN{ FS = "/" } {
      a=0
      for (i=1; i<=NF; i++) {
        if ($i ~ reg )
         a=1
      }
      if (a != 1) {
        print $0
      }
    }' | xargs file | awk '{$1= ""; print $0}' |
        sort | awk '{ if (length($0) > 40 ) $0=substr($0,0,41)"..."; print $0 }'  | uniq -c | sort -gr)
  else
    pole=$(find -type f -exec file {} + | awk '{$1= ""; print $0}' |
      sort | awk '{ if (length($0) > 40 ) $0=substr($0,0,41)"..."; print $0 }'  | uniq -c | sort -gr)
  fi
  oldIFS="$IFS"
  IFS="\n"

  # divide number of files and type
  pocet=$(printf "$pole" | awk '{print $1}')
  nazov=$(printf "$pole" | awk '{$1= ""; print substr($0,2,length($0));}' )

  # only first ten filetypes
  pocet1=$(printf $pocet | head | tr '\n' ';')
  nazov1=$(printf "$nazov" | head | tr '\n' ';')
  IFS=";"
  j=1

  # formanted filename and amount to array
  for i in $pocet1 ; do
    pocet2[$j]=$((i))
    j=$(($j+1))
  done

  j=1
  for i in $nazov1 ; do
    nazov2[$j]="$i"
    j=$(($j+1))
  done  

  # normalization
  if [ $NORM_FORMA = "true" ] ; then
    max=1
    if [ $((${pocet2[$max]})) -gt $(($COLS_SIZE_N-47)) ]; then
      old_max=$((${pocet2[$max]}))
      pocet2[$max]=$(($COLS_SIZE_N-47))

      i=2
      while [ $(($i)) -le $((${#pocet2[@]})) ]; do
        pocet2[$i]=$(((${pocet2[$i]}*${pocet2[$max]})/$old_max))
        i=$(($i+1))
      done
    fi
  fi 

  # output of second histogram
  j=1
  printf "File type histogram:\n"
if [ $((${pocet2[1]})) -ne 0 ] ; then
  while [ $(($j)) -le $((${#nazov2[@]})) ] ; do
      printf "  %-43s: " ${nazov2[$j]}
    k=1
    while [ $k -le $((pocet2[$j])) ]; do
      printf "#"
      k=$((k+1))
    done
    printf "\n"
    j=$(($j+1))
  done
fi 
}


# ***********************************************************************
# MAIN
# ***********************************************************************
# 1). Checking of arguments
# 2). Attempt to change dir
# 3). Call of functions
# -----------------------------------------------------------------------

# 1).
while getopts "ni:" o
  do  case "$o" in
    n)      #echo "Option ’n’ found."
            NORM_FORMA="true"
            ;;
    i)      #echo "Option ’i’ found with parameter ’$OPTARG’."
            REGEXP="true"
            ARG=$OPTARG
            #ARG=$(echo $ARG | sed 's/^\^//;s/\$$//')
            #echo $ARG
            ;;
    *)      printf "Use options n or i with a parameter\n" >&2
            exit 1
            ;;
    esac
  done
shift "$((OPTIND - 1))"

# 2.)
if [ $# -gt 1 ] ; then
  printf "Wrong arguments for path\n">&2
  exit 1
elif [ $# -eq 1 ] ; then
  cesta="$1"
  cd $cesta              
  if [ $? -eq 1 ] ; then  
    printf "Couldnt change dir $cesta\n">&2
    exit 1
  fi
fi

# 3).
basic_info
hist_size   
hist_files

exit 0
