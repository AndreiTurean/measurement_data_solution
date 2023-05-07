# TASK: Implmenet ASC reader functionalities

How to interpter the ASC file and transform data into SW packages. Lets take for example the following line:

```
1669636332.7601 CANFD   4 Rx         2ad  SymbolicName        1 0 a 10 00 00 FA FA 00 00 FA FA 00 80   0  0   123000 0 0 0 
```
After parsing the code line the following information can be extracted:
* Type = CAN (CANFD is not defined as package type for now)
* Timestamp = 16696363327601
* Size = 10 
* Payload = 00 00 FA FA 00 00 FA FA 00 80
* Source Handle = 0xDEAD (use a temp handle for now)
* Cycle = 0x0a

After your implementation the following code shall pass without any errors or fails.

```
    DataPackageCPtr data = reader->readData();

    ASSERT_NE(data, nullptr);

    EXPECT_EQ(data->cycle_, 15);
    EXPECT_EQ(data->size, 10);
    EXPECT_TRUE(std::memcmp(data->payload, "0000FAFA0000FAFA0080", data->size) == 0);
    EXPECT_EQ(data->sourceHandle, 0XDEAD);
    EXPECT_EQ(data->timestamp, 16696363327601);
    EXPECT_EQ(data->type, PackageType::can);

```
