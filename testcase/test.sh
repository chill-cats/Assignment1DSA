for f in tests/*
do
  echo ""
  echo "Running test $f"
  ASAN_OPTIONS=detect_leaks=1 ../build/test "$f"
  hasLeak=$?
  if [[ $hasLeak -ne 0 ]]
  then
    echo "Leak detected"
    break
  fi
done
