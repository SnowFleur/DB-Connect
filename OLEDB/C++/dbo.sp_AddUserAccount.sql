GO
/****** Object:  StoredProcedure [dbo].[sp_AddUserAccount]    Script Date: 2022-07-17 21:54:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER PROCEDURE [dbo].[sp_AddUserAccount](
	@char_ID	NCHAR(10),		-- ID
	@char_PW	NCHAR(10)		-- PW
)

AS

DECLARE @reuslt INT
SET @reuslt=0	--0은 성공


BEGIN

BEGIN TRAN

	INSERT INTO dbo.UserAccount VALUES(@char_ID,@char_PW)

	SET @reuslt= @@ERROR

	if @reuslt <> 0
		ROLLBACK TRAN
	else
		COMMIT TRAN

	--return @reuslt
END








