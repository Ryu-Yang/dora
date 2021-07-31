use std::ffi::CString;
use std::sync::Arc;

use anyhow::Result;

use crate::context::Context;
use crate::init_options::InitOptions;

/// Initialize rclrust context
///
/// # Examples
///
/// ```
/// let ctx = rclrust::init().unwrap();
/// assert!(ctx.valid());
/// ```
pub fn init() -> Result<Arc<Context>> {
    init_with_options(InitOptions::new()?)
}

/// Initialize rclrust context
///
/// # Examples
///
/// ```
/// use rclrust::InitOptions;
///
/// let init_options = InitOptions::new().unwrap();
/// let ctx = rclrust::init_with_options(init_options).unwrap();
/// assert!(ctx.valid());
/// ```
pub fn init_with_options(init_options: InitOptions) -> Result<Arc<Context>> {
    Context::new(
        &std::env::args()
            .map(CString::new)
            .collect::<std::result::Result<Vec<_>, _>>()?,
        init_options,
    )
}

/// Check rclrust's status.
pub fn ok(ctx: Arc<Context>) -> bool {
    ctx.valid()
}

/// Shutdown rclrust context
pub fn shutdown(ctx: Arc<Context>, reason: &str) -> Result<()> {
    ctx.shutdown(reason)
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn rclrust_init() -> Result<()> {
        let ctx = init()?;
        assert!(ok(ctx));

        Ok(())
    }
}
