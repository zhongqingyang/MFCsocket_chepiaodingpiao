USE [master]
GO
/****** Object:  Database [train]    Script Date: 02/21/2019 21:11:00 ******/
CREATE DATABASE [train] ON  PRIMARY 
( NAME = N'train', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\train.mdf' , SIZE = 3072KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'train_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\train_log.ldf' , SIZE = 1024KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
GO
ALTER DATABASE [train] SET COMPATIBILITY_LEVEL = 100
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [train].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [train] SET ANSI_NULL_DEFAULT OFF
GO
ALTER DATABASE [train] SET ANSI_NULLS OFF
GO
ALTER DATABASE [train] SET ANSI_PADDING OFF
GO
ALTER DATABASE [train] SET ANSI_WARNINGS OFF
GO
ALTER DATABASE [train] SET ARITHABORT OFF
GO
ALTER DATABASE [train] SET AUTO_CLOSE OFF
GO
ALTER DATABASE [train] SET AUTO_CREATE_STATISTICS ON
GO
ALTER DATABASE [train] SET AUTO_SHRINK OFF
GO
ALTER DATABASE [train] SET AUTO_UPDATE_STATISTICS ON
GO
ALTER DATABASE [train] SET CURSOR_CLOSE_ON_COMMIT OFF
GO
ALTER DATABASE [train] SET CURSOR_DEFAULT  GLOBAL
GO
ALTER DATABASE [train] SET CONCAT_NULL_YIELDS_NULL OFF
GO
ALTER DATABASE [train] SET NUMERIC_ROUNDABORT OFF
GO
ALTER DATABASE [train] SET QUOTED_IDENTIFIER OFF
GO
ALTER DATABASE [train] SET RECURSIVE_TRIGGERS OFF
GO
ALTER DATABASE [train] SET  DISABLE_BROKER
GO
ALTER DATABASE [train] SET AUTO_UPDATE_STATISTICS_ASYNC OFF
GO
ALTER DATABASE [train] SET DATE_CORRELATION_OPTIMIZATION OFF
GO
ALTER DATABASE [train] SET TRUSTWORTHY OFF
GO
ALTER DATABASE [train] SET ALLOW_SNAPSHOT_ISOLATION OFF
GO
ALTER DATABASE [train] SET PARAMETERIZATION SIMPLE
GO
ALTER DATABASE [train] SET READ_COMMITTED_SNAPSHOT OFF
GO
ALTER DATABASE [train] SET HONOR_BROKER_PRIORITY OFF
GO
ALTER DATABASE [train] SET  READ_WRITE
GO
ALTER DATABASE [train] SET RECOVERY FULL
GO
ALTER DATABASE [train] SET  MULTI_USER
GO
ALTER DATABASE [train] SET PAGE_VERIFY CHECKSUM
GO
ALTER DATABASE [train] SET DB_CHAINING OFF
GO
EXEC sys.sp_db_vardecimal_storage_format N'train', N'ON'
GO
USE [train]
GO
/****** Object:  Table [dbo].[User]    Script Date: 02/21/2019 21:11:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[User](
	[UseZH] [nvarchar](50) NOT NULL,
	[UseSex] [nvarchar](50) NULL,
	[UsePhone] [nvarchar](50) NULL,
	[UsePwd] [nvarchar](50) NULL,
	[money] [nvarchar](50) NULL,
	[UseName] [nvarchar](50) NULL,
	[UseID] [nvarchar](50) NULL,
	[UseQX] [nvarchar](50) NULL,
	[UserQX] [nvarchar](max) NULL
) ON [PRIMARY]
GO
INSERT [dbo].[User] ([UseZH], [UseSex], [UsePhone], [UsePwd], [money], [UseName], [UseID], [UseQX], [UserQX]) VALUES (N'test1', N'男', N'1561564521', N'1', N'1143', N'朱奎', N'350821199712143412', N'用户', N'')
INSERT [dbo].[User] ([UseZH], [UseSex], [UsePhone], [UsePwd], [money], [UseName], [UseID], [UseQX], [UserQX]) VALUES (N'test2', N'男', N'1561564522', N'1', N'19880', N'章朝佩', N'350821199712143413', N'用户', NULL)
INSERT [dbo].[User] ([UseZH], [UseSex], [UsePhone], [UsePwd], [money], [UseName], [UseID], [UseQX], [UserQX]) VALUES (N'admin', N'男', N'1564564561', N'1', N'2960', N'钟庆杨', N'350821199723123223', N'高级管理员', N'0,1,1,1,1')
INSERT [dbo].[User] ([UseZH], [UseSex], [UsePhone], [UsePwd], [money], [UseName], [UseID], [UseQX], [UserQX]) VALUES (N'admin1', N'女', N'1216545645', N'1', NULL, N'郑祖祥', N'350821199564564565', N'订单管理员', N'1,0,0,0,0')
INSERT [dbo].[User] ([UseZH], [UseSex], [UsePhone], [UsePwd], [money], [UseName], [UseID], [UseQX], [UserQX]) VALUES (N'admin2', N'男', N'4564156456', N'1', NULL, N'赵维方', N'3505645456454564', N'车辆管理员', N'0,0,0,1,0')
INSERT [dbo].[User] ([UseZH], [UseSex], [UsePhone], [UsePwd], [money], [UseName], [UseID], [UseQX], [UserQX]) VALUES (N'admin3', N'女', N'456456456213', N'1', NULL, N'尚明明', N'35456156456564', N'用户管理员', N'0,1,0,0,0')
INSERT [dbo].[User] ([UseZH], [UseSex], [UsePhone], [UsePwd], [money], [UseName], [UseID], [UseQX], [UserQX]) VALUES (N'admin4', N'男', N'12121212', N'1', NULL, NULL, NULL, N'高级管理员', N'0,1,1,1,1')
INSERT [dbo].[User] ([UseZH], [UseSex], [UsePhone], [UsePwd], [money], [UseName], [UseID], [UseQX], [UserQX]) VALUES (N'test3', N'男', N'1561564522', N'1', N'19840', N'章朝佩', N'350821199712143413', N'用户', NULL)
/****** Object:  Table [dbo].[Train]    Script Date: 02/21/2019 21:11:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Train](
	[trainID] [nvarchar](50) NOT NULL,
	[Start] [nvarchar](50) NULL,
	[Finish] [nvarchar](50) NULL,
	[GoTime] [nvarchar](50) NULL,
	[OverTime] [nvarchar](50) NULL,
	[ZT] [numeric](18, 0) NULL,
	[YT] [numeric](18, 0) NULL,
	[Price] [numeric](18, 0) NULL,
	[banci] [nvarchar](50) NULL
) ON [PRIMARY]
GO
INSERT [dbo].[Train] ([trainID], [Start], [Finish], [GoTime], [OverTime], [ZT], [YT], [Price], [banci]) VALUES (N'K215', N'常州', N'南京', N'19.30', N'20.30', CAST(300 AS Numeric(18, 0)), CAST(300 AS Numeric(18, 0)), CAST(40 AS Numeric(18, 0)), N'2019-3-12')
INSERT [dbo].[Train] ([trainID], [Start], [Finish], [GoTime], [OverTime], [ZT], [YT], [Price], [banci]) VALUES (N'K215', N'常州', N'南京', N'19.30', N'20.30', CAST(300 AS Numeric(18, 0)), CAST(300 AS Numeric(18, 0)), CAST(40 AS Numeric(18, 0)), N'2019-3-13')
INSERT [dbo].[Train] ([trainID], [Start], [Finish], [GoTime], [OverTime], [ZT], [YT], [Price], [banci]) VALUES (N'K215', N'常州', N'南京', N'19.30', N'20.30', CAST(300 AS Numeric(18, 0)), CAST(300 AS Numeric(18, 0)), CAST(40 AS Numeric(18, 0)), N'2019-3-11')
/****** Object:  Table [dbo].[Oder]    Script Date: 02/21/2019 21:11:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Oder](
	[OrderID] [int] IDENTITY(1,1) NOT NULL,
	[UserZH] [nvarchar](50) NULL,
	[trainID] [nchar](10) NULL,
	[trainBegin] [nvarchar](50) NULL,
	[trainEnd] [nvarchar](50) NULL,
	[OrderTime] [nvarchar](50) NULL,
	[Price] [nvarchar](50) NULL,
	[Seat] [nvarchar](50) NULL
) ON [PRIMARY]
GO
SET IDENTITY_INSERT [dbo].[Oder] ON
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (81, N'test1', N'K215      ', N'2018-10-9 11.30', N'2018-10-9 12.30', N'2018-10-09 22:58:53 ', N'40', N'01,5')
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (67, NULL, NULL, NULL, NULL, NULL, NULL, N'01,005')
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (68, N'test2', N'K215      ', N'2018-9-28 8.30', N'2018-9-28 9.30', N'2018-09-26 18:21:13 ', N'40', N'01,003')
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (82, N'test1', N'K215      ', N'2018-10-10 8.30', N'2018-10-10 9.30', N'2018-10-09 23:01:43 ', N'40', N',2')
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (83, N'test1', N'K215      ', N'2018-10-11 8.30', N'2018-10-11 9.30', N'2018-10-09 23:02:19 ', N'40', N',1')
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (71, N'test2', N'K215      ', N'2018-10-7 19.30', N'2018-10-7 20.30', N'2018-10-07 15:30:53 ', N'40', N'01,003')
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (72, N'test2', N'K215      ', N'2018-10-8 15.30', N'2018-10-8 16.30', N'2018-10-07 15:31:38 ', N'40', N'01,003')
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (73, N'test2', N'K215      ', N'2018-10-9 11.30', N'2018-10-9 12.30', N'2018-10-07 15:31:54 ', N'40', N'01,003')
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (74, N'test3', N'K215      ', N'2018-10-7 19.30', N'2018-10-7 20.30', N'2018-10-07 15:33:23 ', N'40', NULL)
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (75, N'test3', N'K215      ', N'2018-10-9 11.30', N'2018-10-9 12.30', N'2018-10-07 15:39:35 ', N'40', N'01,4')
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (77, N'test3', N'K215      ', N'2018-10-8 15.30', N'2018-10-8 16.30', N'2018-10-07 16:08:38 ', N'40', N'01,4')
INSERT [dbo].[Oder] ([OrderID], [UserZH], [trainID], [trainBegin], [trainEnd], [OrderTime], [Price], [Seat]) VALUES (78, N'test3', N'K215      ', N'2018-10-10 8.30', N'2018-10-10 9.30', N'2018-10-07 16:11:11 ', N'40', N',1')
SET IDENTITY_INSERT [dbo].[Oder] OFF
/****** Object:  Table [dbo].[Admin]    Script Date: 02/21/2019 21:11:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Admin](
	[ID] [nvarchar](50) NOT NULL,
	[name] [nvarchar](50) NULL,
	[Pwd] [nvarchar](50) NULL,
	[Phone] [nvarchar](50) NULL
) ON [PRIMARY]
GO
INSERT [dbo].[Admin] ([ID], [name], [Pwd], [Phone]) VALUES (N'admin', N'郑祖祥', N'1', N'15295138537')
INSERT [dbo].[Admin] ([ID], [name], [Pwd], [Phone]) VALUES (N'002', N'钟庆杨', N'zqy', N'15295138536')
