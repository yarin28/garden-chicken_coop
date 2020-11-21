void setup() {
  Serial.begin(9600);
  int myPins[] = {2, 4, 8, 3, 6};
  int arr[10] = {9, 3, 2, 4, 3, 2, 7, 8, 9, 11};
  char* = intarrInToString(arr, 10);


}
char intarrInToString(int[] arr , int len ) {
  char str[len];
  int i = 0;
  int index = 0;
  for (i = 0; i < 5; i++)
    index += snprintf(&str[index], 128 - index, "%d", a[i]);
  return str;
}
void loop() {
  Serial.print("Hello from Arduino!");
  delay(2000);

1}
